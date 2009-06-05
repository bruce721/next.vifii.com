prefix=/opt/hildon
exec_prefix=${prefix}
bindir=${exec_prefix}/bin

all: hildon-session.sh start-hildon.sh

hildon-session.sh: hildon-session.sh.in

install:
	install -d -m755 $(DESTDIR)$(bindir)
	install -m755 hildon-session.sh start-hildon.sh $(DESTDIR)$(bindir)/

uninstall:
	rm -f $(DESTDIR)$(bindir)/hildon-session.sh $(DESTDIR)$(bindir)/start-hildon.sh

clean:
	-rm hildon-session.sh