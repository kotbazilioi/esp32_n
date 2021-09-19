#!python

#ver.3.3
#22.03.2010

#ver.3.4
#31.05.2010

#ver.3.5-52
#30.03.2011
#compile_time moved inside of function

#ver.3.6-52
#27.07.2011
#files with names starting with '_' placed into code segment

#ver 4.0-253
#30.10.2011
#rework of pages storage, every page as char[] array, referenced from header structure by name

#ver 5.1-52
#11.05.2012
#files placed into the code segment by explicit including into the 'residents' array (names without path)

#ver. 5.2-52
#16.06.2016
#added cachable_types with 'ico' for favicon.ico

#ver. 5.3-54
#28.08.2017
#input and output file paths tied to argv[0] (running script location)

#ver.5.4
#21.06.2018 by LBS
#embedded mib and template files

#ver.5.5
#31.08.2021 by LBS
# ported to python 3.9

import os, sys, time
import gzip
#from string import replace # python 3.9 compatibility, 31.08.2021 LBS
import shortcut ## LBS made

HTML_FLG_GET         = 0x01
HTML_FLG_POST        = 0x02
HTML_FLG_COMPRESSED  = 0x04
HTML_FLG_NOCACHE     = 0x08
HTML_FLG_CODESEG     = 0x10
HTML_FLG_CGI         = 0x40

compressed_types = ['html', 'css', 'ico', 'xml', 'oidlib', 'mib']
##compressed_types = ['html', 'css', 'ico']
cachable_types = ['ico']
embedded_file_types = ['xml', 'mib', 'oidlib'] # served as bytes with 'application/octet-stream' type

def ext(fn):
    return os.path.splitext(fn)[1][1:]

def pack_html_pages(res_dir, output_file, residents = []):    
    compiled_time = time.ctime()
    res_dir = os.path.abspath(os.path.dirname(sys.argv[0]) + '\\' + res_dir)
    flist = os.listdir(res_dir)
    
    fo = open(os.path.dirname(sys.argv[0]) + '\\' + output_file, 'w')
    print('// NetPing HTML resource compiler v4', file=fo)
    print('// HTML pages data for HTTP2 module\r\n', file=fo)
    print('// Source dir is ' + os.path.abspath(res_dir) + '\r\n', file=fo)
    print('// Compiled ' + compiled_time + '\r\n\r\n', file=fo)
    print('#include "html/html.h"', file=fo)
    data_size_codeseg = 0
    data_size_mt = 0
    page_headers = []
    for fn in flist:
        flags = HTML_FLG_GET
        if ext(fn) not in cachable_types:
          flags |= HTML_FLG_NOCACHE
        full_fn = os.path.join(res_dir,fn)
        if(ext(fn) == 'lnk'): ## windows shortcut
          full_fn = shortcut.resolve(full_fn)
          fn = fn[:-4] ## strip .lnk extension
        p = open(full_fn,'rb').read()
        if ext(fn) in compressed_types:
            p = gzip.compress(p)
            flags |= HTML_FLG_COMPRESSED
        page_len = len(p)
        p = b'\x59\x95' + p ## prepend header 0x59 0x95 - page signature
        if fn in residents:
          flags |= HTML_FLG_CODESEG

        data_name = fn.replace('.', '_')
        data_name = data_name.replace(' ', '_')
        data_name = data_name.replace('[', '_')
        data_name = data_name.replace(']', '_')

        segment = '' ### ' @ "MT" '
        if flags & HTML_FLG_CODESEG:
            segment = ''            
        print('\nconst __attribute__((used)) char _html_%s[] %s =' % ( data_name, segment), file=fo)
        print('{', ','.join([str(b) for b in p ]), '};', file=fo) # html resource binary data, optionally gziped
        data_size_codeseg += page_len
        mime_type = 'mime_bin' if ext(fn) in embedded_file_types else 'mime_'+ext(fn)
        print('struct np_html_page_s _html_page_%s = { %s, _html_%s+2, %d, %#.04x };\n' % ( data_name, mime_type, data_name, page_len, flags ), file=fo) # uri context for ESP httpd API
        fn_escaped = fn.replace(' ','%20')
        page_headers.append('{ "/%s", HTTP_GET, np_http_get_handler, (void*)&_html_page_%s }' % ( fn_escaped, data_name )) # collect uri structures for ESP httpd API
    pass
    print('const httpd_uri_t np_html_uri[] = {\n', ',\n'.join(page_headers), '\n};\n', file=fo)
    print('#define NP_HTML_HEADERS_NUMBER   ', len(page_headers), '\n', file=fo)    
    fo.close()

    print("\n\n", compiled_time, file=sys.stderr)
    print("Processing of HTML resources completed successfully.", file=sys.stderr)
    print("Output file "+os.path.abspath(output_file), file=sys.stderr)
    print("Data size (without headers) in Code segment: ", data_size_codeseg, '\n', file=sys.stderr)

pass ## end pack_html_pages()

