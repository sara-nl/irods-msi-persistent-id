Feature: replication

  Scenario: wordcount of one file
    Given an object /tempZone/home/rods/example.txt from example.txt
    Then wordcount of /tempZone/home/rods/example.txt is 42
