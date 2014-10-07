LATEX = pdflatex
BIBTEX = bibtex
BUILD_DIR = build
SCRIPTS_DIR = scripts
WORDCOUNT_SCRIPT = texcount.pl
PERL = perl
FILE = report
PDF = $(FILE).pdf

.PHONY: clean distclean pdf wordcount quick

all: pdf

quick:
	mkdir -p $(BUILD_DIR)
	$(LATEX) -file-line-error -output-directory=$(BUILD_DIR) -interaction=nonstopmode -synctex=1 $(FILE).tex
	mv $(BUILD_DIR)/$(PDF) $(PDF)

pdf:
	mkdir -p $(BUILD_DIR)
	rm -f $(BUILD_DIR)/$(FILE).bbl
	$(LATEX) -file-line-error -output-directory=$(BUILD_DIR) -interaction=nonstopmode -synctex=1 $(FILE).tex
	$(BIBTEX) $(BUILD_DIR)/$(FILE).aux
	$(LATEX) -file-line-error -output-directory=$(BUILD_DIR) -interaction=nonstopmode -synctex=1 $(FILE).tex
	$(LATEX) -file-line-error -output-directory=$(BUILD_DIR) -interaction=nonstopmode -synctex=1 $(FILE).tex
	mv $(BUILD_DIR)/$(PDF) $(PDF)


clean:
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -f $(FILE).pdf

wordcount:
	$(PERL) $(SCRIPTS_DIR)/$(WORDCOUNT_SCRIPT) -inc $(FILE).tex

