# An example of a client/server program

This is a server/client, where the server receives messages, prints them and reply to the corresponding client with a message ``"Received"``. Clients ask the user for  message, send it to the server, print the response and exit.


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
