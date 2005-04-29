<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
		version="1.0">
	
	<!-- Basic stylesheet: standard DocBook chunked HTML -->
	<xsl:import href="/usr/share/sgml/docbook/xsl-stylesheets/html/chunk.xsl"/>

	<!-- Local overrides -->
	<!-- Number sections, but only the first two levels; from DocBook FAQ -->
	<xsl:param name="section.autolabel" select="1"/>
	<xsl:param name="toc.section.depth" select="2"/>
	<xsl:param name="section.label.includes.component.label" select="1"/>
	<xsl:param name="local.l10n.xml" select="document('')"/>
	<l:i18n xmlns:l="http://docbook.sourceforge.net/xmlns/l10n/1.0">
	  <l:l10n language="en">
		<l:context name="title-numbered">
		  <l:template name="sect3" text="%t"/>
		  <l:template name="sect4" text="%t"/>
		  <l:template name="sect5" text="%t"/>
		</l:context>

		<l:context name="section-xref-numbered">
		  <l:template name="sect3" 
		  text="the section called &#8220;%t&#8221;"/>
		  <l:template name="sect4" 
		  text="the section called &#8220;%t&#8221;"/>
		  <l:template name="sect5" 
		  text="the section called &#8220;%t&#8221;"/>
		</l:context>

	  </l:l10n>
	</l:i18n>
</xsl:stylesheet>
