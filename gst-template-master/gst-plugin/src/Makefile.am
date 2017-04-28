# Note: myfilterdir is set in configure

##############################################################################
# TODO: change libgstmyfilter.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstmyfilter.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

# sources used to compile this plug-in
libgstmyfilter_la_SOURCES = gstmyfilter.c gstmyfilter.h

# compiler and linker flags used to compile this myfilter, set in configure.ac
libgstmyfilter_la_CFLAGS = $(GST_CFLAGS)
libgstmyfilter_la_LIBADD = $(GST_LIBS)
libgstmyfilter_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstmyfilter_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = gstmyfilter.h
