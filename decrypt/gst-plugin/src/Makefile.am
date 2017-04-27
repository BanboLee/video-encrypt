# Note: decryptdir is set in configure

##############################################################################
# TODO: change libgstdecrypt.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstdecrypt.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

# sources used to compile this plug-in
libgstdecrypt_la_SOURCES = gstdecrypt.c gstdecrypt.h speck.c speck.h

# compiler and linker flags used to compile this decrypt, set in configure.ac
libgstdecrypt_la_CFLAGS = $(GST_CFLAGS)
libgstdecrypt_la_LIBADD = $(GST_LIBS)
libgstdecrypt_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstdecrypt_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstdecrypt.h
