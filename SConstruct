import os

headers = ['itg3200.h']
sources = ['itg3200.c']

lib = Library('libitg3200', sources)

install_prefix = '/usr/local'
install_include = os.path.join(install_prefix, 'include')
install_lib = os.path.join(install_prefix, 'lib')

Alias('install', Install(install_include, headers))
Alias('install', Install(install_lib, lib))
Command('uninstall', None, Delete(FindInstalledFiles()))

Default(lib)
