# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

import subprocess

def configureDoxyfile(input_dir1, input_dir2, output_dir):

    with open('Doxyfile.in', 'r') as file :
        filedata = file.read()

    filedata = filedata.replace('@DOXYGEN_INPUT_DIR1@', input_dir1)
    filedata = filedata.replace('@DOXYGEN_INPUT_DIR2@', input_dir2)
    filedata = filedata.replace('@DOXYGEN_OUTPUT_DIR@', output_dir)

    with open('Doxyfile', 'w') as file:
        file.write(filedata)

breathe_projects = {}
input_dir1 = '../macOS'
input_dir2 = '../include'
output_dir = 'build'
configureDoxyfile(input_dir1, input_dir2, output_dir)
subprocess.call('doxygen', shell=True)
breathe_projects['feeling-blue'] = output_dir + '/xml'



# -- Project information -----------------------------------------------------

project = 'feeling-blue'
copyright = '2020, sean ng pack'
author = 'sean ng pack'

# The full version, including alpha/beta/rc tags
release = '1.0'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [ "breathe" ]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

html_theme_options = {
    'navigation_depth': 4,
    'collapse_navigation': False
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# Breathe Configuration
breathe_default_project = "feeling-blue"