//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#include <fstream>
#include <sstream>
#include "common.h"
#include "connector.h"
#include "mmap.h"
#include "param.h"
#include "utils.h"

namespace MeCab {

bool Connector::open(const Param &param) {
  const std::string filename = create_filename
      (param.get<std::string>("dicdir"), MATRIX_FILE);
  return open(
      filename.c_str(),
      param.get<std::string>("left-space-penalty-factor").c_str());
}

bool Connector::open(const char* filename,
                     const char* left_space_penalty_factor_str,
                     const char *mode) {
  CHECK_FALSE(cmmap_->open(filename, mode))
      << "cannot open: " << filename;

  matrix_ = cmmap_->begin();

  CHECK_FALSE(matrix_) << "matrix is NULL" ;
  CHECK_FALSE(cmmap_->size() >= 2)
      << "file size is invalid: " << filename;

  lsize_ = static_cast<unsigned short>((*cmmap_)[0]);
  rsize_ = static_cast<unsigned short>((*cmmap_)[1]);

  CHECK_FALSE(static_cast<size_t>(lsize_ * rsize_ + 2)
                    == cmmap_->size())
      << "file size is invalid: " << filename;

  matrix_ = cmmap_->begin() + 2;

  set_left_space_penalty_factor(left_space_penalty_factor_str);
  return true;
}

void Connector::set_left_space_penalty_factor(const char *factor_str) {
  if (factor_str == NULL) return;

  char s[512];
  snprintf(s, sizeof(s), "%s", factor_str);

  const size_t max_num_space_penalty_pos_ids = 64;
  char *col[max_num_space_penalty_pos_ids];
  const size_t n = tokenizeCSV(s, col, max_num_space_penalty_pos_ids);
  for (size_t i = 0; i < n; i += 2) {
    left_space_penalty_factor_.push_back(
        SpacePenalty(
            (unsigned short )strtoul(col[i], NULL, 0),
            (int )strtol(col[i+1], NULL, 0)));
  }
}

void Connector::close() {
  cmmap_->close();
}

int Connector::cost(const Node *lNode, const Node *rNode) const {
  return matrix_[lNode->rcAttr + lsize_ * rNode->lcAttr] +
      rNode->wcost +
      get_space_penalty_cost(rNode);
}

int Connector::get_space_penalty_cost(const Node *rNode) const {
  if (rNode->rlength == rNode->length) {
    // has no space
    return 0;
  }
  for (size_t i = 0; i < left_space_penalty_factor_.size(); ++i) {
    if (rNode->posid == left_space_penalty_factor_[i].posid_) {
      return this->left_space_penalty_factor_[i].penalty_cost_;
    }
  }
  return 0;
}

bool Connector::openText(const char *filename) {
  std::ifstream ifs(WPATH(filename));
  if (!ifs) {
    WHAT << "no such file or directory: " << filename;
    return false;
  }
  char *column[2];
  scoped_fixed_array<char, BUF_SIZE> buf;
  ifs.getline(buf.get(), buf.size());
  CHECK_DIE(tokenize2(buf.get(), "\t ", column, 2) == 2)
      << "format error: " << buf.get();
  lsize_ = std::atoi(column[0]);
  rsize_ = std::atoi(column[1]);
  return true;
}

bool Connector::compile(const char *ifile, const char *ofile) {
  std::ifstream ifs(WPATH(ifile));
  std::istringstream iss(MATRIX_DEF_DEFAULT);
  std::istream *is = &ifs;

  if (!ifs) {
    std::cerr << ifile
              << " is not found. minimum setting is used." << std::endl;
    is = &iss;
  }


  char *column[4];
  scoped_fixed_array<char, BUF_SIZE> buf;

  is->getline(buf.get(), buf.size());

  CHECK_DIE(tokenize2(buf.get(), "\t ", column, 2) == 2)
      << "format error: " << buf.get();

  const unsigned short lsize = std::atoi(column[0]);
  const unsigned short rsize = std::atoi(column[1]);
  std::vector<short> matrix(lsize * rsize);
  std::fill(matrix.begin(), matrix.end(), 0);

  std::cout << "reading " << ifile << " ... "
            << lsize << "x" << rsize << std::endl;

  while (is->getline(buf.get(), buf.size())) {
    CHECK_DIE(tokenize2(buf.get(), "\t ", column, 3) == 3)
        << "format error: " << buf.get();
    const size_t l = std::atoi(column[0]);
    const size_t r = std::atoi(column[1]);
    const int    c = std::atoi(column[2]);
    CHECK_DIE(l < lsize && r < rsize) << "index values are out of range";
    progress_bar("emitting matrix      ", l + 1, lsize);
    matrix[(l + lsize * r)] = static_cast<short>(c);
  }

  std::ofstream ofs(WPATH(ofile), std::ios::binary|std::ios::out);
  CHECK_DIE(ofs) << "permission denied: " << ofile;
  ofs.write(reinterpret_cast<const char*>(&lsize), sizeof(unsigned short));
  ofs.write(reinterpret_cast<const char*>(&rsize), sizeof(unsigned short));
  ofs.write(reinterpret_cast<const char*>(&matrix[0]),
            lsize * rsize * sizeof(short));
  ofs.close();

  return true;
}
}
