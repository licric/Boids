COMANDI PIù UTILI 

->git push is the command used to update the remote
repository (local->remote)  //fails of remote version is bigger (newer) than local

->git fetch allows to pull commits, files and refs from the
remote repository without merging with the local (local<-remote)

->git pull will fetch the files and also merge them with the
current state of your workspace (local directory<-local repository)

->git status 




SE SCRIVO UN NUOVO FILE

$ git add "filename"

$ git commit -m "n^th Commit"

$ git push



CLONARE TUTTA LA CARTELLA ONLINE GIT SUL PC

$ git clone git@github.com:licric/Boids_programmazione_24.git



CREARE UN NUOVO BRANCH

$ git branch "new_branch"

$ git branch                   //mostra tutti i branch



CAMBIARE BRANCH

->se faccio commit i salvataggi vengono fatti sul branch attivo

$ git switch "branch_name"     //cambia branch



MERGA TWO BRANCHES

$ git switch main            //first switch to destination branch

$ git merge "branch_name"    //unisce main e "branch_name"

->Git can automatically merge commits unless there are changes
that conflict in both commit sequences



ALTRI COMANDI

git log shows the local repository history

git diff is used to show the changes in the files
