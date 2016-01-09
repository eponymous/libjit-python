#!/usr/bin/env python
import os
from distutils.core import setup, Extension

jit = Extension('jit',
                sources = ['src/jit-block.cpp',
                           'src/jit-context.cpp',
                           'src/jit-dynamic.cpp',
                           'src/jit-function.cpp',
                           'src/jit-jump-table.cpp',
                           'src/jit-label.cpp',
                           'src/jit-module.cpp',
                           'src/jit-type.cpp',
                           'src/jit-value.cpp'],
                language = 'c++',
                include_dirs=['src/', '/usr/local/include/'],
                libraries = ['jit', 'jitplus', 'jitdynamic'],
                library_dirs = ['/usr/local/lib', '/usr/local/lib64'])

setup(name='pylibjit',
      version="0.1",
      description='Python bindings for libjit',
      author='Dan Eicher',
      author_email='dan@eu.phorio.us',
      ext_modules=[jit])
