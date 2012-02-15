# makefile pro preklad LaTeX verze Bc. prace
# (c) 2008 Michal Bidlo
# E-mail: bidlom AT fit vutbr cz
#===========================================
# asi budete chtit prejmenovat:
PROJECT_NAME=barcodes
TEX_DIR=tex
TMP_DIR=tmp

all: $(PROJECT_NAME).pdf

pdf: $(PROJECT_NAME).pdf

$(TMP_DIR)/$(PROJECT_NAME).ps: $(PROJECT_NAME).dvi
	dvips $(PROJECT_NAME)

$(PROJECT_NAME).pdf: clean
	pdflatex $(PROJECT_NAME)
	bibtex $(PROJECT_NAME)
	pdflatex $(PROJECT_NAME)
	pdflatex $(PROJECT_NAME)

$(PROJECT_NAME).dvi: $(PROJECT_NAME).tex $(PROJECT_NAME).bib
	latex $(PROJECT_NAME)
	bibtex $(PROJECT_NAME)
	latex $(PROJECT_NAME)
	latex $(PROJECT_NAME)

desky:
#	latex desky
#	dvips desky
#	dvipdf desky
	pdflatex desky

clean:
	rm -f *.dvi *.log $(PROJECT_NAME).blg $(PROJECT_NAME).bbl $(PROJECT_NAME).toc *.aux $(PROJECT_NAME).out $(PROJECT_NAME).lof
	rm -f $(PROJECT_NAME).pdf
	rm -f *~

pack:
	tar czvf bp-xjmeno.tar.gz *.tex *.bib *.bst ./fig/* ./cls/* Makefile Changelog
