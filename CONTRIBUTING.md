# Contributor guidelines

To make process smooth as possible, we should follow rules below.

## Organization

### Test code before creating pull request **[VERY IMPORTANT]**

New pull request will trigger travis CI pipeline and will consume
our free build hours. Pushing own branches to repo is allowed, because it does not trigger build pipeline.

Please note **other people** can have same **name of branches**, so **do not overwrite** them.

### Issue planning

Assignee(person who is working on issue) of issue should tag is issues with following tags:

* importance of issue (wontfix | minor | major | critical)
* difficulty points of issue (3, 5, 7, 13 : where 3 is easy, 13 very hard)

### Code sessions

Team code sessions are organized by **xvinar00** and should announced on Discord and Facebook as well.

### Meetings

Team meetings should take place once a week. Each meeting should set new milestone or continue working on existing one.

Before every meeting **xlorin01** will publish new meeting agenda, where you can add own ideas/suggestions.

## Workflow

### Pull requests

Before creating pull request, create issue and link issue to pull request. It can be done by naming pull request `#{issue number} - {pull request name}`.

### Bug reports

Before you create issue (bug report), try to write down steps how to reproduce bug. Tag issue as *bug*.

If you know how to fix bug, save us 😂. 

### Questions

Prefer asking questions on Discord or Facebook. If it is suggestion, after approval of other members create new pull request.

## Project hierarchy

```
project
│   README.md - description of project
│   CONTRIBUTING.md - this file    
│   .travis.yml - CI (Continuous Integration - Pull request checks)│      
│
└───src
    │   Makefile
    │   *.c files of project
    |   *.h header files of project
    |   test suites

   
