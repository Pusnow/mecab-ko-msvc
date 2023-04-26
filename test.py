import MeCab
import sys
import string

sentence = "무궁화꽃이피었습니다."

try:
    print(MeCab.VERSION)
    print("0")
    print(dir(MeCab))
    print(dir(MeCab.Tagger))
    t = MeCab.Tagger()
    print("1", t)
    print(t.parse(sentence))

    m = t.parseToNode(sentence)
    while m:
        print(m.surface, "\t", m.feature)
        m = m.next
    print("EOS")

    lattice = MeCab.Lattice()
    t.parse(lattice)
    lattice.set_sentence(sentence)
    len = lattice.size()
    for i in range(len + 1):
        b = lattice.begin_nodes(i)
        e = lattice.end_nodes(i)
        while b:
            print("B[%d] %s\t%s" % (i, b.surface, b.feature))
            b = b.bnext 
        while e:
            print("E[%d] %s\t%s" % (i, e.surface, e.feature))
            e = e.bnext 
    print("EOS")

    d = t.dictionary_info()
    while d:
        print("filename: %s" % d.filename)
        print("charset: %s" %  d.charset)
        print("size: %d" %  d.size)
        print("type: %d" %  d.type)
        print("lsize: %d" %  d.lsize)
        print("rsize: %d" %  d.rsize)
        print("version: %d" %  d.version)
        d = d.next

except RuntimeError as e:
    print("RuntimeError:", e)
