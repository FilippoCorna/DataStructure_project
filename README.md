# DataStructure_project
String Checker algorithm

# WordChecker

## Description

**WordChecker** is a system developed in C that compares the letters of two words of equal length, verifying the correspondence between their characters. It supports alphanumeric characters, hyphens, and underscores, and handles various commands for adding and comparing words.

## Objectives

The goal of this project is to implement a system capable of:

- Reading a set of admissible words.
- Performing comparisons between a reference word and words to compare.
- Applying constraints learned from the comparisons.
- Printing results and compatible words based on the established constraints.

## Features

- **Main Commands:**
  - `+nuova_partita`: Starts a new game.
  - `+stampa_filtrate`: Prints admissible words that are compatible with the current constraints.
  - `+inserisci_inizio` and `+inserisci_fine`: Add new words to the set of admissible words.

- **Word Comparison:**
  - Returns an output string that indicates the position and correspondence of characters.
  - Supports output states such as `ok`, `ko`, and `not_exists`.
