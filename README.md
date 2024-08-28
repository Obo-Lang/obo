```
@Message:
    @.new(msg): .msg = msg
    @.print(): print(.msg)

@main():
    msg = Message("Hello!")
    msg.print()
    msg.msg = "Bye!", msg.print()

main()
```