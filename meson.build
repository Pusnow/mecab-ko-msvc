project('mecab-ko-msvc', 'cpp',
    version: '0.996.ko-0.9.2',
)


cxx = meson.get_compiler('cpp')


conf = configuration_data()
deps = []

check_headers = [
    'ctype.h',
    'dirent.h',
    'dlfcn.h',
    'fcntl.h',
    'inttypes.h',
    'io.h',
    'memory.h',
    'pthread.h',
    'stdint.h',
    'stdlib.h',
    'string.h',
    'strings.h',
    'sys/mman.h',
    'sys/stat.h',
    'sys/times.h',
    'sys/param.h',
    'sys/types.h',
    'unistd.h',
    'windows.h',
]


check_functions = [
    ['mmap', '#include <sys/mman.h>'],
    ['getenv', '#include <stdlib.h>'],
    ['getpagesize', '#include <unistd.h>'],
]

sizeof_types = [
    'char',
    'int',
    'long',
    'long long',
    'short',
    'size_t',
]

foreach check_header: check_headers
    has = cxx.has_header(check_header)
    name = 'HAVE_' + check_header.to_upper().replace('.','_').replace('/','_')
    conf.set(name, has ? 1 : false, description: 'Define to 1 if you have the <@0@> header file.'.format(check_header))
endforeach

foreach check_function: check_functions
    has = cxx.has_function(check_function[0], prefix: check_function[1])
    name = 'HAVE_' + check_function[0].to_upper().replace('.','_').replace('/','_')
    conf.set(name, has ? 1 : false, description: 'Define to 1 if you have the `@0@\' function.'.format(check_function[0]))
endforeach

foreach sizeof_type: sizeof_types
    size = cxx.sizeof(sizeof_type, prefix : '#include<stdlib.h>')
    name = 'SIZEOF_' + sizeof_type.to_upper().replace(' ','_')
    conf.set(name, size, description: 'The size of `@0@\', as computed by sizeof'.format(sizeof_type))
endforeach



enable_gcc_atomic_ops_code = '''#include <sched.h>
int
main ()
{
  int a = 10;
  __sync_fetch_and_add(&a, 10);
  __sync_val_compare_and_swap(&a, 0, 10);
  sched_yield();
  return 0;
}
'''
enable_gcc_atomic_ops = cxx.compiles(enable_gcc_atomic_ops_code)
conf.set('HAVE_GCC_ATOMIC_OPS', enable_gcc_atomic_ops ? 1 : false)

enable_osx_atomic_ops_code = '''#include <libkern/OSAtomic.h>

int
main ()
{
  int a = 10;
  OSAtomicAdd32(10, &a);
  OSAtomicCompareAndSwapInt(10, 0, &a);
  return 0;
}
'''
enable_osx_atomic_ops = cxx.compiles(enable_osx_atomic_ops_code)
conf.set('HAVE_OSX_ATOMIC_OPS', enable_osx_atomic_ops ? 1 : false)

enable_tls_code = '''__thread int a = 0;

int
main ()
{
  a = 10;
  return 0;
}
'''
enable_tls = cxx.compiles(enable_tls_code)
conf.set('HAVE_TLS_KEYWORD', enable_tls ? 1 : false)


have_unsigned_long_long_int = cxx.sizeof('unsigned long long int', prefix : '#include<stdlib.h>')
conf.set('HAVE_UNSIGNED_LONG_LONG_INT', have_unsigned_long_long_int != -1 ? 1 : false, description : 'Define to 1 if the system has the type `unsigned long long int\'.')

if cxx.sizeof('off_t', prefix : '#include<sys/types.h>') == -1
    conf.set('off_t', 'long int', description : 'Define to `long int\' if <sys/types.h> does not define.')
endif

if cxx.sizeof('size_t', prefix : '#include<sys/types.h>') == -1
    conf.set('size_t', 'unsigned int', description : 'Define to `unsigned int\' if <sys/types.h> does not define.')
endif

words_bigendian = host_machine.endian() == 'big'
conf.set('WORDS_BIGENDIAN', words_bigendian ? 1 : false)

iconvdeps = cxx.find_library('iconv', required: false)
if iconvdeps.found()
    deps += iconvdeps
endif

have_iconv = cxx.has_function('iconv', prefix: '#include <iconv.h>', dependencies: iconvdeps)
conf.set('HAVE_ICONV', have_iconv, description : 'Define if you have the iconv() function and it works.')

iconv_const_code = '''#include <stdlib.h>
#include <iconv.h>
extern
#ifdef __cplusplus
"C"
#endif
#if defined(__STDC__) || defined(__cplusplus)
size_t iconv (iconv_t cd, char * *inbuf, size_t *inbytesleft, char * *outbuf, size_t *outbytesleft);
#else
size_t iconv();
#endif

int
main ()
{

  ;
  return 0;
}'''

if have_iconv
    iconv_const = cxx.compiles(iconv_const_code)
    conf.set('ICONV_CONST', iconv_const ? '' : 'const')
endif

configure_file(output : 'config.h',
               configuration : conf)


add_global_arguments('-DHAVE_CONFIG_H', language : 'cpp')
add_global_arguments('-DDIC_VERSION=102', language : 'cpp')
add_global_arguments('-DPACKAGE="mecab"', language : 'cpp')
add_global_arguments('-DVERSION="@0@"'.format(meson.project_version()), language : 'cpp')

prefixdir  = get_option('prefix')

sysconfdir = join_paths(prefixdir, get_option('sysconfdir'))
includedir = join_paths(prefixdir, get_option('includedir'))
libdir = join_paths(prefixdir, get_option('libdir'))
datadir = join_paths(prefixdir, get_option('datadir'))
bindir = join_paths(prefixdir, get_option('bindir'))

if host_machine.system() == 'windows'
    mecab_default_rc = 'c:\\\\mecab\\\\etc\\\\mecabrc'

    add_global_arguments('-municode', language : 'cpp')
    add_global_arguments('-DUNICODE', language : 'cpp')
    add_global_arguments('-D_UNICODE', language : 'cpp')
    if cxx.has_function_attribute('dllexport')
        add_global_arguments('-DDLL_EXPORT', language : 'cpp')
    endif
else
    mecab_default_rc = '@0@/mecabrc'.format(sysconfdir)
endif
add_global_arguments('-DMECAB_DEFAULT_RC="@0@"'.format(mecab_default_rc), language : 'cpp')


headers = [
    'src/char_property.h',
    'src/common.h',
    'src/connector.h',
    'src/context_id.h',
    'src/darts.h',
    'src/dictionary_rewriter.h',
    'src/dictionary.h',
    'src/feature_index.h',
    'src/freelist.h',
    'src/iconv_utils.h',
    'src/lbfgs.h',
    'src/learner_node.h',
    'src/learner_tagger.h',
    'src/mecab.h',
    'src/mmap.h',
    'src/nbest_generator.h',
    'src/param.h',
    'src/scoped_ptr.h',
    'src/stream_wrapper.h',
    'src/string_buffer.h',
    'src/thread.h',
    'src/tokenizer.h',
    'src/ucs.h',
    'src/ucstable.h',
    'src/ucstable.h',
    'src/utils.h',
    'src/viterbi.h',
    'src/winmain.h',
    'src/writer.h',
]

lib_sources = [
    'src/char_property.cpp',
    'src/connector.cpp',
    'src/context_id.cpp',
    'src/dictionary_compiler.cpp',
    'src/dictionary_generator.cpp',
    'src/dictionary_rewriter.cpp',
    'src/dictionary.cpp',
    'src/eval.cpp',
    'src/feature_index.cpp',
    'src/iconv_utils.cpp',
    'src/lbfgs.cpp',
    'src/learner_tagger.cpp',
    'src/learner.cpp',
    'src/libmecab.cpp',
    'src/nbest_generator.cpp',
    'src/param.cpp',
    'src/string_buffer.cpp',
    'src/tagger.cpp',
    'src/tokenizer.cpp',
    'src/utils.cpp',
    'src/viterbi.cpp',
    'src/writer.cpp',
]


if host_machine.system() == 'windows'
    libmecab_name = 'libmecab'
else
    libmecab_name = 'mecab'
endif

thread_dep = dependency('threads')
deps += thread_dep
libmecab = both_libraries(libmecab_name, lib_sources, dependencies: deps, install : true)
libmecab_static = libmecab.get_static_lib()

if get_option('build_static')
    flags = ['-static']
else
    flags = []
endif


mecab = executable('mecab', 'src/mecab.cpp', link_with: libmecab_static, install : true, c_args : flags, cpp_args : flags, link_args : flags)
mecab_dict_index = executable('mecab-dict-index', 'src/mecab-dict-index.cpp', link_with: libmecab_static, install : true, c_args : flags, cpp_args : flags, link_args : flags)
mecab_dict_gen = executable('mecab-dict-gen', 'src/mecab-dict-gen.cpp', link_with: libmecab_static, install : true, c_args : flags, cpp_args : flags, link_args : flags)
mecab_cost_train = executable('mecab-cost-train', 'src/mecab-cost-train.cpp', link_with: libmecab_static, install : true, c_args : flags, cpp_args : flags, link_args : flags)
mecab_system_eval = executable('mecab-system-eval', 'src/mecab-system-eval.cpp', link_with: libmecab_static, install : true, c_args : flags, cpp_args : flags, link_args : flags)
mecab_test_gen = executable('mecab-test-gen', 'src/mecab-test-gen.cpp', link_with: libmecab_static, install : true, c_args : flags, cpp_args : flags, link_args : flags)


mecabrc_conf = configuration_data()
mecabrc_conf.set('prefix', prefixdir)
configure_file(input : 'mecabrc.in',
               output : 'mecabrc',
               configuration : mecabrc_conf,
               install: true,
               install_dir: get_option('sysconfdir'))


install_headers('src/mecab.h')
install_man('man/mecab.1')


add_userdic_conf = configuration_data()
add_userdic_conf.set('DIC_PATH', join_paths(datadir, 'mecab-ko-dic'))
add_userdic_conf.set('BIN_PATH', bindir)
if host_machine.system() == 'windows'
    configure_file(input : 'tools/add-userdic.ps1',
                   output : 'add-userdic.ps1',
                   configuration : add_userdic_conf,
                   install: true,
                   install_dir: get_option('bindir'))
else
    configure_file(input : 'tools/add-userdic.sh',
                   output : 'add-userdic.sh',
                   configuration : add_userdic_conf,
                   install: true,
                   install_dir: get_option('bindir'))
endif


if get_option('build_dic')
    python_exe = find_program('python3', 'python')
    run_command(python_exe, '-m', 'zipfile', '-e', meson.source_root() / 'mecab-ko-dic.zip', meson.build_root(), check:true)
    mecab_ko_dic = custom_target('mecab_ko_dic',
    output : 'mecab-ko-dic',
    depends : [mecab_dict_index],
    command : [mecab_dict_index, '-d', '@OUTPUT@', '-o', '@OUTPUT@', '-f', 'UTF-8', '-t', 'UTF-8'],
    install : true,
    install_dir : get_option('datadir'))
endif

swig_args = []
if get_option('build_java')
    swig_args += ['-java', '-package', 'org.chasen.mecab']
endif
if get_option('build_python')
    swig_args += ['-python']
    python_conf = configuration_data()
    python_conf.set('VERSION', meson.project_version())
    python_conf.set('INCLUDE_PATH', includedir)
    python_conf.set('LIB_PATH', libdir)
    configure_file(input : 'swig/setup.py.in',
                   output : 'setup.py',
                   configuration : python_conf,
                   install: true,
                   install_dir: datadir / 'swig')

endif
if get_option('build_perl')
    swig_args += ['-perl']
endif
if get_option('build_ruby')
    swig_args += ['-ruby']
endif
if get_option('build_csharp')
    swig_args += ['-csharp', '-namespace', 'MeCab']
endif

if swig_args.length() > 0
    swig = find_program('swig')
    swig_src = 'swig/MeCab.i'
    run_command(swig, swig_args, '-c++', '-o', meson.build_root() /'mecab_wrap.cpp', '-outdir', meson.build_root(), swig_src, check:true)
    install_data(meson.build_root()/'mecab_wrap.cpp', install_dir: datadir / 'swig')
    if get_option('build_python')
        install_data(meson.build_root()/'MeCab.py', install_dir: datadir / 'swig')
    endif
endif
