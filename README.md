*comandi più utili*

->git push is the command used to update the remote
repository (local->remote)  //fails of remote version is bigger (newer) than local

->git fetch allows to pull commits, files and refs from the
remote repository without merging with the local (local<-remote)

->git pull will fetch the files and also merge them with the
current state of your workspace (local directory<-local repository)

->git status 




*scrivo un nuovo file*

$ git add "filename"

$ git commit -m "n^th Commit"

$ git push



*clonare tutta la cartella online git sul pc*

$ git clone git@github.com:licric/Boids_programmazione_24.git



*creare un nuovo branch*

$ git branch "new_branch"

$ git branch                   //mostra tutti i branch



*cambiare branch*

->se faccio commit i salvataggi vengono fatti sul branch attivo

$ git switch "branch_name"     //cambia branch



*merge two branches*

$ git switch main            //first switch to destination branch

$ git merge "branch_name"    //unisce main e "branch_name"

->Git can automatically merge commits unless there are changes
that conflict in both commit sequences



*altri comandi*

git log shows the local repository history

git diff is used to show the changes in the files
