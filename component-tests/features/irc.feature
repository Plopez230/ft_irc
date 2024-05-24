Feature: IRC

    @<filename>
    Scenario Outline: <filename>
        Given a IRC server connection: localhost 6667
        Then replay the trace <filename>

    Examples:
    | filename                                                              |
    | pass.log                                                              |
    | nick.log                                                              |
    | user.log                                                              |
    | join.log                                                              |
    | topic.log                                                             |
    | invite.log                                                            |
    | kick.log                                                              |
    | part.log                                                              |
    | privmsg.log                                                           |
    | mode.log                                                              |
    | quit.log                                                              |
