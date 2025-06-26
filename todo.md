## Sam

- [ ] unmanaged test case (see `tofix.md`)
- [ ] expand `~`
  - [ ] expand `~<USER>`
- [x] print sys error (`perror`)
  - [x] in parser => don't print 2 times
  - [x] in env init
- [ ] heredoc (use pipe instead of tmp file)
  - [ ] init pipe
  - [ ] fork in parent
  - [ ] for each HD NODE write it in pipe

## Mat

- [ ] PATH
  - [ ] empty entrie => .
  - [ ] gestion default value (quand pad d'env)
- [ ] expand `$_` in child (path last cmd) & in parent (last arg last cmd)
