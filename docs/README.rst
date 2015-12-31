Introduction
============

In looking to put together my own project that used Qt's SSL Sockets, I had a hard time
finding an example of the server-side. This project provides an extremely simple example
of both the client and the server for the Qt SSL Sockets - so simple it basically just
establishes the connections. It can be easily modified to do more.

Note: This project does put each connection into its own thread. The GUI therefore should
always remain responsive.

Creating the SSL Key and Certificate
====================================

To utilize the Qt SSL Server provided here you must have an X.509 Certificate and Key.
These can be easily generated with the OpenSSL tool kit as shown in the following example:

.. code-block:: shell

	$ openssl req -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 365

Please beware that the above is only an example and good enough for testing this project.
However, in a real production environment it is best to consult a security professional
to determine the best X.509 certificates to make to ensure their lasting value.


Building the example
====================

Building the project requires very few things - CMake 3.x, Qt 5.x, and a C++ compiler (f.e G++).
CMake will ensure that everything necessary to build the project is available and can
manage out-of-source builds for you. The following is an example of building the code
in a sub-directory called *build*:

.. code-block:: shell

	$ mkdir build
	$ cd build
	$ cmake ..
	$ make


Running the examples
====================

There are two executables that are provided when built. Here's how to run each.

The Server
----------

To start the server:

.. code-block:: shell

	$ cd build
	$ ./server/src/exampleQtSslServer --ssl-key=key.pem --ssl-certificate=cert.pem

This will bring up a simple GUI that shows a *Start* button, a *Stop* button, and a
white text box. The *Start* button opens the port and allows the server to receive
connections; while the *Stop* button closes the port. The white text box will record
any log messages that occur.

The Client
----------

.. code-block:: shell

	$ cd build
	$ ./client/src/exampleQtSslClient

This will bring up a simple GUI that shows a *Start* button, a *Stop* button, and a
white text box. The *Start* button opens the port to the server; while the *Stop* button
closes the connection. The white text box will record any log messages that occur.
