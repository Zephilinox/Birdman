# Birdman The Game 
A game based on the movie Birdman 

## TODO

- splash screen?
	- would be neat, not much effort
- main menu
	- get some proper menu sprites/text.
	- Play/Options(?)/Exit
- game
	- plays
		- audience reaction
		- different nights (preview/actual)
		- dialogue/story/plot
			- 2-3 plays worth?
		- characters
			- names
			- sprites/portraits?
		- UI
			dialogue text box
			option menu buttons/boxes
			fonts
			audience reaction
- game over
	- ?
	- score?
- random number generator
- audio
	- library
		- Irrklang like in tutorial?
		- Or something else?
	- UI
	- background music
	- other SFX (audience reaction?)


(todo: remove before merging with master when game is finished)
## git cheat sheet

Remember to let others know if you're working on existing parts of the codebase, so merge conflicts can be avoided

Alternatively ignore all this, push to develop, and we'll try and sort the mess out later.
Doing it this way should help avoid the mess in the first place, but it adds some overhead to the whole process.

### Starting

Clone the repo

`git clone https://github.com/UWEGames-LLP/birdman-the-game-teambirb.git`

switch to develop

`git checkout develop`

### Check changes so far

`git status`

### Commit them

```
git add --all
git commit -m "title here" -m "body here"
```

### Creating a new feature

Instead of pushing changes to develop, it's better to make a new branch.
This way, people pulling from develop won't have a broken build (i.e. compiler errors)

So, make sure you're on the develop branch by checking it out

Now create your new branch

`git checkout -b branch_name_here`

Make your changes, then push

first time pushing this branch?
`git push -u origin branch_name_here`

otherwise just use `git push`

### Merging your branch to develop

Once you're done with your branch or know that it's not broken, merge it to develop

```
git checkout develop
git merge branch_name_here
git push
```

ideally only finished branches get merged back, but it's okay if it's not finished

### Done with your branch?

If the branch is done with, remember to delete it locally and remotely
todo: google how to do this, can't remember
