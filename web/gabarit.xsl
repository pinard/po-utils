<?xml version="1.0" encoding="utf-8"?>
<stylesheet version="1.0"
            xmlns="http://www.w3.org/1999/XSL/Transform">
  <import href="/home/pinard/entretien/mes-sites/commun.xsl"/>
  <output method="html" encoding="UTF-8"/>
  <template match="/">
    <call-template name="gabarit-entretien">
      <with-param name="long-package-name" select="'Free PO utilities'"/>
      <with-param name="README" select="'/README.html'"/>
      <with-param name="NEWS" select="'/NEWS.html'"/>
      <with-param name="Browse" select="'http://github.com/pinard/po-utils'"/>
      <with-param name="Download" select="'/achives'"/>
      <with-param name="TODO" select="'/TODO.html'"/>
      <with-param name="Contributors" select="'/THANKS.html'"/>
    </call-template>
  </template>
</stylesheet>
