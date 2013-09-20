.PHONY: spell-check clean

all: clean thesis.pdf thesis.ps

thesis.ps: thesis.dvi
	dvips $^ -o thesis.ps

thesis.pdf: thesis.dvi
	dvipdf $^ thesis.pdf

thesis.dvi: spell-check thesis.tex thesis.bbl
	latex thesis.tex

thesis.bbl: sources.bib
	latex thesis
	bibtex thesis
	latex thesis

spell-check:
	find . -name \*.tex -exec aspell --check '{}' \;

clean:
	rm -f *.bak
	rm -f *.ps
	rm -f *.pdf
	rm -f *.dvi
	rm -f *.lo[tfg]
	rm -f *.bbl *.blg
	rm -f *.aux *.toc
