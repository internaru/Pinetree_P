#!/usr/bin/env python

# Gather all the scan files necessary into a single archive. 
#
# Usually called from the oem/common/scan/config/Makefile using the mkdist
# target. By calling from the makefile, the required scan code can be gathered
# without any extras that we might not want to release. 
#
# davep 16-Apr-2014

import sys
import os
import os.path
import logging
import glob
import re
import subprocess

logger = logging.getLogger("mkdist")
#logger = logging.getLogger(__name__)

# name of the ASIC and is the path in common/scan/src/asic/
asic_name = "granite"

zipfile_name = "scan_fw.zip"

path_to_zip = "/usr/bin/zip"

# need this function here because it's called to updated many, many global
# paths
def get_oem_name():
    # search current working directory for a dir named "owm". The OEM name will
    # be the next dir down from the "oem".
    cdir = os.getcwd()
    last_tail = None
    while 1 :
        fields = os.path.split(cdir)
        if fields[1]=="oem":
            break

        cdir = fields[0]
        last_tail = fields[1]

    return last_tail

oem_name = get_oem_name()
oem_scan_path = os.path.join("oem",oem_name,"common","scan")

# Gathering some of the files is a bit tricky because there are two separate
# builds we need to collect: the kernel and the appspace tools.  Some of the
# code is shared between kernel and appspace. But the two need separate header
# files. So there are some duplicate header file names (one for kernel, one for
# userspace). We have to carefully separate search paths for the kernel,
# userspace so we find all the correct header files.

# All top-level scan code directories where include files might be hiding. 
kernel_header_file_paths = ( 
    "common/scan/src",
    "common/scan/src/asic/{0}".format(asic_name),
    "common/scan/include",
    "common/scan/lsp/kernel/include",
    "oem/{0}/common/scan/config".format(oem_name),
)

appspace_header_file_paths = ( 
    "common/scan/src",
    "common/scan/src/asic/{0}".format(asic_name),
    "common/scan/include",
    "common/scan/chimera/include",
)

# Most code is captured by the makefile passing a list of filenames through the
# command line. The following are locations of code that must be included in
# the release but are not part of the kernel build.
#
# Globbing is allowed and encouraged.

# oem apps source
oem_app_src = { "basename" : os.path.join(oem_scan_path,"apps"),
                "file_list": ('copyapp.[ch]', 
                              'copy_config.h',
                              'scanapp.[ch]',
                              'copyappcmd.c',
                              'makefile' ),
                "includes" : appspace_header_file_paths,
              }
                
oem_config_src = { "basename" : os.path.join(oem_scan_path,"config"),
                   "file_list": ("Makefile",
                                 "uploadscan", ),
                   "includes" : kernel_header_file_paths,
                 }

# Some of the programs outside the kernel kscantask need some files from the
# core scan code.
core_common_src = {"basename" : os.path.join("common","scan","src"),
                   "file_list": ( "grayline.[ch]",
                                  "scos.c",
                                  "mbuffer.[ch]",
                                  "scanmem.[ch]",
                                  "scanman.[ch]",
                                  "scanmanobs.[ch]",
                                  "scanman_smirb.[ch]",
                                  "adfsensor_stub.c",
                                  "scansen_stub.c",
                                  "afe_stub.c",
                                ),
                "includes" : appspace_header_file_paths,
              }


# Linux userspace test/debug applications plus appspace code that is built into
# the main MFP elf.
core_app_src = {"basename" : os.path.join("common","scan","lsp","apps"),
                "file_list": ( "adfsensor_appspace.c",
                                "cmdline.c",
                                "copy_to_host.c",
                                "outfile.[ch]",
                                "runscan.c",
                                "scancmd.c",
                                "scanlib_appspace.c",
                                "scanmech_app.[ch]",
                                "scanplat_linux.c",
                                "scansen_appspace.c",
                                "scan_stubs.c",
                                "scansysfs.h",
                                "makefile",
                                "apps.mk",
                                "mkdevscan",
                                ),
                "includes" : appspace_header_file_paths,
              }

libfloat_src = { "basename" : os.path.join("common","scan","lsp","kernel","libfloat"),
                 "file_list": (
                 "./test/floattest.c",
                 "./test/Makefile",
                 "./test/ftest.c",
                 "./fplib_glue.S",
                 "./processors/SPARC-gcc.h",
                 "./processors/386-gcc.h",
                 "./processors/ARM-gcc.h",
                 "./Makefile",
                 "./softfloat/softfloat.txt",
                 "./softfloat/bits64/softfloat-macros",
                 "./softfloat/bits64/ARM-gcc/timesoftfloat",
                 "./softfloat/bits64/ARM-gcc/softfloat.h",
                 "./softfloat/bits64/ARM-gcc/Makefile",
                 "./softfloat/bits64/ARM-gcc/milieu.h",
                 "./softfloat/bits64/ARM-gcc/softfloat-specialize",
                 "./softfloat/bits64/templates/softfloat.h",
                 "./softfloat/bits64/templates/Makefile",
                 "./softfloat/bits64/templates/milieu.h",
                 "./softfloat/bits64/templates/softfloat-specialize",
                 "./softfloat/bits64/SPARC-Solaris-gcc/softfloat.h",
                 "./softfloat/bits64/SPARC-Solaris-gcc/Makefile",
                 "./softfloat/bits64/SPARC-Solaris-gcc/milieu.h",
                 "./softfloat/bits64/SPARC-Solaris-gcc/softfloat-specialize",
                 "./softfloat/bits64/timesoftfloat.c",
                 "./softfloat/bits64/386-Win32-gcc/softfloat.h",
                 "./softfloat/bits64/386-Win32-gcc/Makefile",
                 "./softfloat/bits64/386-Win32-gcc/milieu.h",
                 "./softfloat/bits64/386-Win32-gcc/softfloat-specialize",
                 "./softfloat/bits64/softfloat.c",
                 "./softfloat/timesoftfloat.txt",
                 "./softfloat/bits32/softfloat-macros",
                 "./softfloat/bits32/ARM-gcc/softfloat.h",
                 "./softfloat/bits32/ARM-gcc/softfloat.s.1",
                 "./softfloat/bits32/ARM-gcc/softfloat.s",
                 "./softfloat/bits32/ARM-gcc/Makefile",
                 "./softfloat/bits32/ARM-gcc/milieu.h",
                 "./softfloat/bits32/ARM-gcc/softfloat-specialize",
                 "./softfloat/bits32/templates/softfloat.h",
                 "./softfloat/bits32/templates/Makefile",
                 "./softfloat/bits32/templates/milieu.h",
                 "./softfloat/bits32/templates/softfloat-specialize",
                 "./softfloat/bits32/SPARC-Solaris-gcc/softfloat.h",
                 "./softfloat/bits32/SPARC-Solaris-gcc/Makefile",
                 "./softfloat/bits32/SPARC-Solaris-gcc/milieu.h",
                 "./softfloat/bits32/SPARC-Solaris-gcc/softfloat-specialize",
                 "./softfloat/bits32/timesoftfloat.c",
                 "./softfloat/bits32/386-Win32-gcc/softfloat.h",
                 "./softfloat/bits32/386-Win32-gcc/Makefile",
                 "./softfloat/bits32/386-Win32-gcc/milieu.h",
                 "./softfloat/bits32/386-Win32-gcc/softfloat-specialize",
                 "./softfloat/bits32/softfloat.c",
                 "./softfloat/README.txt",
                 "./softfloat/softfloat-source.txt",
                 ),
                 "includes" : () 
              }

# Chimera is a shim layer that allows part of the scan code to run on a Linux
# system. Used by the standalone test/debug applications.
chimera_app_src = {"basename" : os.path.join("common","scan","chimera","src"),
                   "file_list": ( "minprintf.c",
                                  "memory.c",
                                  "debug.c",
                                ),
                   "includes" : appspace_header_file_paths,
                 }

# aspscan is the Linux/OSX scan-to-host program
aspscan_app_src = {"basename" : os.path.join("common","scan","aspscan","src"),
                   "file_list": ( "aspscan.c",
                                   "log.c",
                                   "xassert.c",
                                   "hdump.c",
                                   "cmdline.c",
                                   "str.c",
                                   "asp.c",
                                   "usbtools.[ch]",
                                   "aspusb.[ch]",
                                   "rgb2lab.c",
                                   "xrgbtorgb.c",
                                   "Makefile",
                                ),
                   # throw in the path to our aspscan headers
                   "includes" : (os.path.join("common","scan","aspscan","include"),),
                 }

aspscan_python = {"basename" : os.path.join("common","scan","aspscan"),
                   "file_list": (   "aspcmdline.py",
                                    "aspnums.py",
                                    "asp.py",
                                    "autoconv.py",
                                    "calparse.py",
                                    "cisximage.py",
                                    "colcut.py",
                                    "depipetap.py",
                                    "icelite.py",
                                    "imgmagick.py",
                                    "log.py",
                                    "miscmath.py",
                                    "mkint.py",
                                    "netscan.py",
                                    "piltool.py",
                                    "random_scan.py",
                                    "scanerr.py",
                                    "scan.py",
                                    "test_netscan.py",
                                ),
                   "includes" : (),
                 }

pytools_python = {"basename" : os.path.join("common","scan","pytools"),
                   "file_list": (   "basename.py",
                                    "bits.py",
                                    "chipgap.py",
                                    "cisx.py",
                                    "color.py",
                                    "colorwalk.py",
                                    "fftimg.py",
                                    "flatfile.py",
                                    "gnuplot.py",
                                    "graygrid.py",
                                    "gretag.py",
                                    "hexdump.py",
                                    "hist.py",
                                    "htonl.py",
                                    "httpish.py",
                                    "icetest.py",
                                    "imsplit.py",
                                    "imtools.py",
                                    "line.py",
                                    "netsend.py",
                                    "parsesiq.py",
                                    "picdescdma.py",
                                    "pichw.py",
                                    "picmarg.py",
                                    "picpcdescdma.py",
                                    "pic.py",
                                    "piecs.py",
                                    "piecstats.py",
                                    "piedescdma.py",
                                    "piedma.py",
                                    "pierows.py",
                                    "piexy.py",
                                    "plotdat.py",
                                    "psesd.py",
                                    "regdump.py",
                                    "register.py",
                                    "rgbtogray.py",
                                    "scanalyzer.py",
                                    "scandump.py",
                                    "scif.py",
                                    "sclkper.py",
                                    "scmd.py",
                                    "wm8152.py",
                                ),
                   "includes" : (),
                 }
                                    
scanclocks_files = { "basename" : os.path.join("common","scan","pytools","scanclocks"),
                     "file_list": ( "scanclocks.html",
                                    "scanclocks.js",
                                    "scanclocks.py",
                                    "scfg1.html",
                                    "scfg1.js", ),
                     "includes" : (),
                   }

afecfg_files = { "basename" : os.path.join("common","scan","pytools","afecfg"),
                 "file_list": ( "wm8152.html",
                                "wm8152.js",
                              ),
                 "includes" : (),
               }

mfpy_files = { "basename" : os.path.join("common","scan","pytools","mfpy"),
                 "file_list": ( "imglab.py",
                                "imgrgb.py",
                                "imgycc.py",
                                "makesiq.py",
                                "npcsc.py",
                                "nptcns.py",
                                "picbdr.py",
                              ),
                 "includes" : (),
               }

def make_hardcoded_filelist(srcfile_hash):
    # take filename src hash and convert into paths using the basename from
    # the hash

    logger.debug("make_hardcoded_filelist()")

    file_list = []
    for f in srcfile_hash["file_list"]:
        path = os.path.join( srcfile_hash["basename"], f )
        
        logger.debug(path)
        logger.debug( glob.glob(path) )

        file_list.extend( glob.glob(path) )

    return file_list

def make_distro_filelist( filenames ) : 
    # Clean/convert the filenames we get from the makefile from relative paths
    # with lots of ../../.. to a relative path from the top of the build tree.
    # Basically we just strip off the ..'s

    logger.debug("make_distro_filelist()")

    # build separate lists to make test/debugging easier
    oem_file_list = []
    common_file_list = []

    for f in filenames:
        if "common/scan" in f :
            path = f[ f.index("common/scan") : ] 
            common_file_list.append(path)
        else:
            oem_file_list.append(os.path.join(oem_scan_path,"config",f))

    file_list = []
    file_list.extend(oem_file_list)
    file_list.extend(common_file_list)

    return file_list

def get_includes(infilename):
    # Read a C source. Find all lines that start with #include. Find the header
    # file in the #include.

    def parse_include(s):
        logger.debug("parse_include() s={0}".format(s))
        # get a header file from a preprocessor #include 
        filenames = re.findall(include_regex,s)
        assert len(filenames)==1,filenames
        return filenames[0]

    include_regex = re.compile( '#include\s+["<]([^">]+)[">]' )

    # load the entire C source into memory; we'll search it for #include's
    with open(infilename,"r") as infile:
        lines = [ s.strip() for s in infile.readlines() ]

    header_files = [ parse_include(s) for s in lines if s.startswith("#include") ]

    logger.debug("infilename={0} num_lines={1} num_header={2}".format(
                infilename,len(lines),len(header_files)))

    return header_files

def find_headers(infilename,header_file_path_list):
    # Find all #includes in this file. For all those header files, search the
    # scan code include paths for those files. 
    #
    # Return a list of discovered header files. Files will have full paths from
    # top of build tree (e.g., "common/scan/include/scantypes.h")

    header_files = get_includes(infilename)
    
    # use a hash to avoid duplicates
    header_file_hash = {}

    # for all the #include's we found in the source...
    for h in header_files :
        # ...search for the header file in the usual paths
        for header_path in header_file_path_list : 
            header_name = os.path.join( header_path, h )
            if os.path.exists(header_name):
                logger.info("found header={0}".format(header_name))
                header_file_hash[header_name] = 1
                # use first file discovered; we split the header file paths
                # between kernel and appspace to solve the duplicate header
                # filename problem
                break
        else :
            # we'll ignore headers we can't find (e.g., string.h and other
            # system headers)
            logger.info("header={0} not found".format(h))
            logger.info("required by {0}".format(infilename))
            logger.debug("searched {0}".format(":".join(header_file_path_list)))

    return header_file_hash

def find_header_files(file_list,header_file_path_list):

    # use a hash to eliminate duplicates
    header_file_hash = {}

    for f in file_list:
        # not a source file, ignore
        if not (f.endswith(".c") or f.endswith(".h")) : 
            continue

        header_file_hash.update(find_headers(f,header_file_path_list))

    # search the newly discovered include files for includes 
    header_file_list = header_file_hash.keys()
    for f in header_file_list : 
        header_file_hash.update(find_headers(f,header_file_path_list))

    return header_file_hash

def test_file_list(output_file_list):
    # chdir to top of build
    # make sure each file exists

    for f in output_file_list:
        assert os.path.exists(f), f 

def main() : 
    # Usually we're called from the linux kernel kscantask makefile. The
    # makefile will pass us a list of all the files used in the build. This is
    # the source of 90% of the code in the Linux scan release.
    filenames = sys.argv[1:]

    # we don't want libraries in our distribution
    filenames = [ f for f in filenames if not f.endswith(".a") ]

    # go to top of build tree; we'll zip up all necessary scan code relative to
    # the top of the build
    cwd = os.getcwd()
    os.chdir("../../../../..")

    logger.info("oem_name={0}".format(oem_name))

    # Note below I'm building a hash of file paths. I'm using a hash to
    # eliminate duplicates.

    # convert filenames from the Makefile into paths (removes all the ../..'s
    # and turns into paths from the top of the build tree)
    distro_files_list = make_distro_filelist(filenames)
    all_files_hash = dict.fromkeys(distro_files_list,1)

    # find all headers required by this batch of files
    all_files_hash.update( find_header_files(distro_files_list,kernel_header_file_paths) )

    # add the global hardwired names
    for other_src in ( oem_app_src, oem_config_src, core_common_src, 
                        core_app_src, chimera_app_src, aspscan_app_src ) : 
        # convert the filenames into path+filename
        file_list = make_hardcoded_filelist(other_src)

        # update our big hash of all files
        all_files_hash.update( dict.fromkeys(file_list,1) )

        # find all headers required by this batch of files
        all_files_hash.update( find_header_files(file_list,other_src["includes"]) )

    # throw in the Python paths
    for python_src in ( aspscan_python, pytools_python, 
                        scanclocks_files, afecfg_files, mfpy_files ): 
        all_files_hash.update(dict.fromkeys(make_hardcoded_filelist(python_src),1))

    # libfloat wants to come along, too!
    all_files_hash.update(dict.fromkeys(make_hardcoded_filelist(libfloat_src),1))

    # top level scan makefile; didn't fit in anywhere else
    all_files_hash[os.path.join("oem",oem_name,"common","scan","makefile")] = 1

    # turn into a flat list 
    all_files_list = all_files_hash.keys()

    # sanity check the files we just created
    test_file_list(all_files_list)

    # if zip already exists, get rid of it
    if os.path.exists(zipfile_name):
        os.unlink(zipfile_name)

    # zip all the files together
    args = [ path_to_zip, "-rq", zipfile_name, ]
    args.extend(all_files_list)
    subprocess.check_call(args,shell=False)
    logger.info("wrote {0}".format(zipfile_name))

    # go back where we were
    os.chdir(cwd)

    with open("mkdist.log","w") as outfile:
        outfile.write("\n".join(all_files_list))

if __name__=='__main__':
#    logging.basicConfig(level=logging.INFO)
    logging.basicConfig(level=logging.DEBUG)
    main()

