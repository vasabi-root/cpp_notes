source: https://proglib.io/p/git-for-half-an-hour

# Git installation
...

# Git setting
`git config --global user.name "vasabi-root"`  
`git config --global user.email vasabi.ph@gmail.com`  

`git config list`  

`git config --global color.ui true`  
`git config --global color.status auto`  
`git config --global color.branch auto`  

# Git repo
`mkdir repo`  
`cd repo`  
`git init`  

`git status`  



## Files preparation
### Local
`git add file.cpp`  
`git add -A dir`  
`git reset file.cpp`  

`git status` -- можно посмотреть после каждой команды, что изменилось

### Remote
`git remote add origin https://github.com/vasabi-root/repo.git` -- `origin` -- типичное название основного репоза


## Commit
### Local
`git commit -m 'awesome commit'`  

`git log` -- история коммитов с хэшами  
`git show hash_commit` -- отследить историю операции по хэшу  

`git commit --amend -m 'new comment'` -- перезаписать последний коммент к коммиту (опасная штука, лучше делать до залива на сервер)

`git rm -r --cached path/to/dir` -- перестать трекать папку (коммит не нужен)

### Remote
`git push origin master` -- отправка изменений из локалки на сервер -- следует отправлять изменения, которые являются реализацией пунктов задачи (не нужно постоянно пушить в репоз). Ветка `master` главная  
`git pull origin master` -- загрузка репоза из сервера на локалку

## Branching
Текущая ветка по умолчанию -- `master`
`git branch my_impementation` -- создание новой ветки  
`git branch` -- просмотр всех доступных веток и текущей ветки (*)  
`git checkout my_impementation` -- выбор текущей ветки

### Merging
`git checkout master`  
`git merge my_impementation` -- слияние `my_impementation` с текущей веткой. После слияния, ветку которую слили, можно удалять

### Deletion
`git checkout not_my_impementation` -- текущую ветку невозможно удалить  
`git branch -d my_impementation`  


## .gitignore
Надо создать файл `.gitignore` в каталоге проекта и перечислить в нём все файлы и папки, которые мы не хотим коммитить:
```c
*.log
*.exe
tokens.txt
.venv/
.vscode/
*test_data/
literature/
dataset/
```
При этом важно отметить, что сам `.gitignore` коммитится и отправляется на сервер -- так и надо! А то как пользователи поймут, каких файлов не достаёт проекту для нормальной работы...
