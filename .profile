set -o vi

PATH="$HOME/scripts:$PATH"
PS1="[\h \W] \$ "
export LC_COLLATE="C"

export HISTCONTROL=ignoredups
export HISTFILE=$HOME/.sh_history
export HISTSIZE=10000

# Standard stuff and variables for my scripts
export EDITOR=vi
export VISUAL=vi
#export XEDIT=some-gui-editor  # Do not set to "xedit", recursion
export NOTIFY=notify
export OPENER=open-file
export BROWSER=firefox
export XDG_CONFIG_HOME="$HOME/.config"
export XDG_DATA_HOME="$HOME/.local/share"
export XDG_CACHE_HOME="$HOME/.cache"

# Program-specific stuff
export FZF_DEFAULT_OPTS="-e -i -m --no-bold --no-color --layout=reverse-list"
export LYNX_CFG="$XDG_CONFIG_HOME/lynx/lynx.cfg"
export LYNX_LSS="$XDG_CONFIG_HOME/lynx/lynx.lss"
export PYTHON_COLORS=0

### Aliases and functions
alias bc='bc -q /home/sebastiano/box/bc.library'
alias o='open-file'
alias rm='trash rm'
alias ls='ls --color=auto'
alias clip='clip -m fzf'
alias bm='dmenu-bookmarks -m fzf'
alias f='feed get && feed -m fzf menu'
alias svi='vi "+so $HOME/.virc-spaces"'

# Email management (mblaze)
MAIL=$HOME/mail
msync() { mbsync -a; }
msc()   { d=${1:-$MAIL/INBOX}; minc -q $d; mlist -t $d | mthread | mseq -S; mscan; }
mget()  { mbsync -a; msc; }
mtt()   { echo $MAIL/Trash | xargs mrefile $@; }
mta()   { echo $MAIL/Archive | xargs mrefile $@; }
mpk()   { f=$1; shift; mpick -t "$f =~~ \"$@\""; }
s()     { mshow $@ | templess; }
sht()   { mshow -A text/html $@; }
n()     { mshow .+1 | templess; }

# nissy build options
#export NISSY_BUILD_SANITIZE="address,undefined"

# Session cookies are kept in .ssh
[ -f $HOME/.ssh/sessions ] && . $HOME/.ssh/sessions
