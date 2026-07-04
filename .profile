# Standard  options and environment variables
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

# Aliases
alias o='open-file'
alias f='feed get && feed -m fzf menu'

alias bc='bc -q /home/sebastiano/box/bc.library'
alias rm='trash rm'
alias ls='ls --color=auto'
alias clip='clip -m fzf'
alias svi='vi "+so $HOME/.virc-spaces"'

# Network management
alias w='iwctl station wlan0 get-networks'
alias e='doas ip link set eth1 up && doas udhcpc -i eth1 -n'
alias d='iwctl station wlan0 disconnect'

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

# startx if in tty1
[ "$(tty)" = "/dev/tty1" ] && \rm -f .serverauth.* && startx
