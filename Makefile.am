AUTOMAKE_OPTIONS = gnits
ACLOCAL_AMFLAGS = -I m4
SUBDIRS = m4 lib src
BABYL = ../rmail/*

install-stow:
	$(MAKE) install \
	  prefix=$(exec_prefix)/stow/$(PACKAGE)-$(VERSION) \
	  exec_prefix=$(exec_prefix)/stow/$(PACKAGE)-$(VERSION)
	rm -f $(exec_prefix)/stow/$(PACKAGE)-$(VERSION)/info/dir
	cd $(exec_prefix)/stow && stow $(PACKAGE)-$(VERSION)

ifneq "$(wildcard ~/etc/mes-sites/site.mk)" ""

site: site-all

package_name = po-utils
rootdir = $(HOME)/GitHub/po-utils/web
margin_color = "\#a3bdd7"
caption_color = "\#dedeff"

SITE_ROOT = 1

include ~/etc/mes-sites/site.mk

endif
