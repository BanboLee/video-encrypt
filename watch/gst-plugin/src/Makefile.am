# Note: watchdir is set in configure

##############################################################################
# TODO: change libgstwatch.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstwatch.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

# sources used to compile this plug-in
libgstwatch_la_SOURCES = gstwatch.c gstwatch.h

# compiler and linker flags used to compile this watch, set in configure.ac
libgstwatch_la_CFLAGS = $(GST_CFLAGS)
libgstwatch_la_LIBADD = $(GST_LIBS)
libgstwatch_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstwatch_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstwatch.h
