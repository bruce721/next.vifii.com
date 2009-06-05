hildonuipolicylib_LTLIBRARIES = libtasknavigatorpolicy.la libstatusbarpolicy.la

libtasknavigatorpolicy_la_LDFLAGS = -module -avoid-version

libtasknavigatorpolicy_la_LIBADD = \
	$(LIBHILDONDESKTOP_LIBS)   \
        $(GTK_LIBS)

libtasknavigatorpolicy_la_CFLAGS =                               \
	-std=c99 -rdynamic                                       \
   	$(LIBHILDONDESKTOP_CFLAGS)                               \
        $(GTK_CFLAGS) 	                                         \
	-DHD_TN_ENTRY_PATH=\"$(hildonnavigatordesktopentrydir)\" 

libtasknavigatorpolicy_la_SOURCES = \
	tasknavigator-policy.c

libstatusbarpolicy_la_LDFLAGS = -module -avoid-version

libstatusbarpolicy_la_LIBADD = \
	$(LIBHILDONDESKTOP_LIBS)   \
        $(GTK_LIBS)

libstatusbarpolicy_la_CFLAGS =                                   \
	-std=c99 -rdynamic                                       \
   	$(LIBHILDONDESKTOP_CFLAGS)                               \
        $(GTK_CFLAGS) 	                                         \
	-DHD_SB_ENTRY_PATH=\"$(hildonstatusbardesktopentrydir)\" 

libstatusbarpolicy_la_SOURCES = \
	statusbar-policy.c

CLEANFILES = *~
