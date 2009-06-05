
MAINTAINERCLEANFILES	= Makefile.in \
			  libhildondesktop-sections.txt \
			  libhildondesktop-undocumented.txt \
			  version.xml \
			  tmpl/*.sgml

DOC_MODULE		= libhildondesktop

DOC_MAIN_SGML_FILE	= $(DOC_MODULE)-docs.sgml

SCAN_OPTIONS		=

SCANOBJ_OPTIONS		=--type-init-func="gtk_type_init(0)"

DOC_SOURCE_DIR		= $(top_srcdir)/libhildondesktop
DOC_BUILD_DIR		= $(top_builddir)/libhildondesktop


HFILE_GLOB		= $(top_srcdir)/libhildondesktop/*.h

CFILE_GLOB		= $(top_srcdir)/libhildondesktop/*.c

#IGNORE_HFILES 					= hildon-banner-private.h

INCLUDES		= $(GTK_CFLAGS) $(HILDON_CFLAGS) -I$(top_srcdir)

MKDB_OPTIONS		= --sgml-mode --output-format=xml

content_files		= version.xml 
#			  visual_index.xml

GTKDOC_CFLAGS          = $(HILDON_CFLAGS)      \
                         $(HILDON_FM_CFLAGS)   \
                         $(HILDON_HELP_CFLAGS) \
                         $(GCONF_CFLAGS)       \
                         $(GNOME_VFS_CFLAGS)   \
                         $(DBUS_GLIB_CFLAGS)

GTKDOC_LIBS            = $(HILDON_LIBS)                                        \
                         $(HILDON_FM_LIBS)                                     \
                         $(OSSO_LIBS)                                          \
                         $(GCONF_LIBS)                                         \
                         $(HILDON_HELP_LIBS)                                   \
                         $(GNOME_VFS_LIBS)                                     \
                         $(DBUS_GLIB_LIBS)                                     \
                         $(top_srcdir)/libhildondesktop/libhildondesktop.la    \
                         $(top_srcdir)/libhildonwm/libhildonwm.la

HTML_IMAGES		=

html: html-build.stamp

version.xml: $(top_srcdir)/configure.ac
	@echo $(VERSION) > version.xml

FIXXREF_OPTIONS		=

include $(top_srcdir)/doc/gtk-doc.make

