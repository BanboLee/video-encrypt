# Note: encryptdir is set in configure

##############################################################################
# TODO: change libgstencrypt.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstencrypt.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

# sources used to compile this plug-in
libgstencrypt_la_SOURCES = gstencrypt.c gstencrypt.h speck.c speck.h

# compiler and linker flags used to compile this encrypt, set in configure.ac
libgstencrypt_la_CFLAGS = $(GST_CFLAGS)
libgstencrypt_la_LIBADD = $(GST_LIBS)
libgstencrypt_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstencrypt_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstencrypt.h
