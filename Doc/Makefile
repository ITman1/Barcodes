# makefile pro preklad LaTeX verze Bc. prace
# (c) 2008 Michal Bidlo
# E-mail: bidlom AT fit vutbr cz
#===========================================
# Upravil: Ing. Jaroslav Dytrych
#===========================================
# asi budete chtit prejmenovat:
SHELL=cmd.exe
CO=QrReader
OUT_DIR=out

# Including OS Specifics Library
include ../GNUMake_OSSLib/include.mk

all: $(OUT_DIR)/$(CO).pdf

pdf: $(OUT_DIR)/$(CO).pdf

$(OUT_DIR)/$(CO).ps: $(OUT_DIR)/$(CO).dvi
	dvips $(CO)

$(OUT_DIR)/$(CO).pdf: reset
	pdflatex -output-directory=$(OUT_DIR) $(CO)
	bibtex $(OUT_DIR)/$(CO)
	pdflatex -output-directory=$(OUT_DIR) $(CO)
	pdflatex -output-directory=$(OUT_DIR) $(CO)

$(OUT_DIR)/$(CO).dvi: $(CO).tex $(CO).bib
	latex -output-directory=$(OUT_DIR) $(CO)
	bibtex $(CO)
	latex -output-directory=$(OUT_DIR) $(CO)
	latex -output-directory=$(OUT_DIR) $(CO)

desky:
#	latex desky
#	dvips desky
#	dvipdf desky
	$(call oss_mkdir,$(OUT_DIR))
	pdflatex -output-directory=$(OUT_DIR) desky

spojit:
	mv $(CO).pdf zprava.pdf
	pdftk A=zprava.pdf B=zadani.pdf cat A1-2 B1-end A3-end output $(CO).pdf

clean:
	$(call oss_rmdir,$(OUT_DIR))

reset: clean
	$(call oss_mkdir,$(OUT_DIR))

pack:
	tar czvf bp-xjmeno.tar.gz *.tex *.bib *.bst ./fig/* ./cls/* $(CO).pdf Makefile Changelog
