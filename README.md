# DemoPlugin #

**DemoPlugin** is a **UG4-Plugin** that shows some very basic functions and classes.
It may be a good entry point for beginners. As a base for more complex plugins
we would however recommend to take a look at 'TemplatePlugin'.

Copyright 2011-2017 Goethe Center for Scientific Computing, University Frankfurt

Please install/clone this repository through UG4's package manager
[ughub](https://github.com/UG4/ughub):

    ughub install DemoPlugin

You can now activate the plugin and build it along with UG4 in the same way as
the other plugins, too. To this go to your build-directory and type:

    cmake -DDemoPlugin=ON .
