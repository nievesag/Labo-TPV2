# An example of a client/server program

This is a server/client, where the server receives messages, prints them and reply to the corresponding client with a message ``"Received"``. The Client asks the user for  messages, in a loop, send it to the server, and print the response. The client exists when the message is ``"exit"``.

The example ``client_server_2_udp_with_serialize.cpp`` is a modification in which the server receives numbers and an arithmetic operation, calculate the result, and send it back. It can also receive a request for a random number.


# How to executes these examples

We will assume that your project is called ``TPV2`` and it is in ``C:\hlocal\TPV2``. Add an example to you project (since all have a ``main`` method, you can include only one at a time).

- To execute the server, open a console and then type

```
  cd c:
  cd c:\hlocal\TPV2\bin
  DebugTPV2.exe server 2000
```

- To execute the client, open another console and the type
  
```
  cd c:
  cd c:\hlocal\TPV2\bin
  DebugTPV2.exe client localhost 2000
```
  
- To get usage information execute:

```
  cd c:
  cd c:\hlocal\TPV2\bin
  DebugTPV2.exe client localhost 2000
```


# How to open a console

- Press ``Windows+R`` (or openb the ``Start`` menu and select ``Run``)
- Type ``cmd`` in the text field and click ``OK`` (or press ``ENTER``)  


More information: [https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/](https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/).
