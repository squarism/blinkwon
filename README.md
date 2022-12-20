## Blinkwon

A hodge-podge of scripts to do ambient lighting with LEDs.


## Install

Good luck.  :|  I just made this for myself over many years.

There are control scripts in `scripts`.  Make these executable.  Change the shell if you need to.  Change the `squarism/blinkwon` (and similar) in the scripts and the `.ino` files to some namespace you want to own.  You can't push to mine.  Also, you'll need an API token too from particle.  Put that in your shell profile.  All of this should run on a Linux server, that's where your schedule runs.  Then flash a bunch of particle boards with the `.ino` files.

When you flash a node, set the options at the top like LED type, quantity and the zone you want to put it into.  Zones are explained in _Concepts_.

The code is generally a mess.  There's a `hasSwitchChanged` thing in the `control.ino` file that isn't used.  Debug statements all over the place.  Ugh.  All I can say is ... it's been running for years.  I've spent some time fighting memory leaks but I think I have it now.


### Background

Another LED light control project.  This one has evolved slowly and organically over time.  The use of this project to you is probably limited.


### Hardware

The system is based around arduino compatible particle.io boards.  Particle comes with an event system and cloud IDE which is free (currently).  It's been working great for more than 8 years.

The particle boards are seated [in custom PCBs](https://oshpark.com/shared_projects/AuTqyGUw) made at OSH Park.  You supply the LEDs, you supply the power.  It easily runs 60 or 120 LEDs per strip.  I even have one strip over ethernet so the controller


### Concepts

1. Light schedule - there's a theme on a cronjob setting whatever schedule you want.
2. Multiple zones - main and accent.  Accent lights are in corners and nooks.
3. Controller - There's a physical circuit to give physical control of the lights.  They are haphazardly on a breadboard.  This physical control just controls another particle board.  There are theme buttons on it and a switch.  The switch is "movie mode".
4. Movie mode - disable the schedule while watching a movie.  This allows you to dim the lights and have it stick.  When the movie is over, flip it back.  The "off" command ignores movie mode.
5. The schedule also includes an off theme which turns off the lights.
6. A node is an arduino running a strip.  There are many.  All they need is wifi and power.  Nothing special but not an ESP32 and relies on particle events.


### The Good

- ☀️ Extendable.  When Siri allowed for SSH commands, I suddenly gained Siri control.  Awesome!  All you have to do is make a voice shortcut to run an SSH command.
- ☀️ Personal.  Putting lights in strategic places is easy.  I use [adafruit ring kits](https://www.adafruit.com/product/2268) for tiny accent lights.
- ☀️ Stable.  Except for wifi problems, it doesn't break.  It hits the Internet so it's really Internet problems.  This is annoying.  If your ISP goes out, you're in the dark.


### The Bad

- ☹️ Particle changed ownership.  You could run your own server maybe (I have not).
- ☹️ Manual editing of the schedule.  Changing themes from halloween to xmas or something is completely manual.  Text edit a cron file.
- ☹️ Power in strange places.  LED strips need a power brick.  The Adafruit ring needs USB.  So, need some USB hub or USB extension cables.


### Schedule Example

You can customize this the way you want (more hours, more steps).  I like having it fade out before midnight.  The thing to watch out for is peak brightness.  One of my strips crashes when it is cranked up.  This makes this touchy.

```
# m h  dom mon dow   command
MAIN_COLOR=orange
ACCENT_COLOR=blacklight
SECONDARY_MAIN_COLOR=blacklight
SECONDARY_ACCENT_COLOR=orange

BASE_BRIGHTNESS=30
ACCENT_BRIGHTNESS=30
INCREMENT=10
ACCENT_INCREMENT=10

# the main zone
00 16 * * * ~/bin/main_color $MAIN_COLOR $BASE_BRIGHTNESS
00 22 * * * ~/bin/main_color $SECONDARY_MAIN_COLOR $(($BASE_BRIGHTNESS + $(($INCREMENT * 7)) ))
40 23 * * * ~/bin/main_color $SECONDARY_MAIN_COLOR $(($BASE_BRIGHTNESS + $(($INCREMENT * 2)) ))
50 23 * * * ~/bin/off

# the accent zone
00 16 * * * ~/bin/accent_color $ACCENT_COLOR $ACCENT_BRIGHTNESS
00 22 * * * ~/bin/accent_color $SECONDARY_ACCENT_COLOR $(($ACCENT_BRIGHTNESS + $(($ACCENT_INCREMENT * 12)) ))
50 23 * * * ~/bin/accent_color $SECONDARY_ACCENT_COLOR $(($ACCENT_BRIGHTNESS + $(($ACCENT_INCREMENT * 1)) ))
00 00 * * * ~/bin/accent_color off

# winter time darkness defense system :P
# work mornings
00 8 * * 1-5 ~/bin/main_color white 10
00 8 * * 1-5 ~/bin/accent_color white 10
00 10 * * 1-5 ~/bin/off
00 10 * * 1-5 ~/bin/accent_color off

# weekends
00 8 * * 6-7 ~/bin/main_color white 40
00 8 * * 6-7 ~/bin/accent_color white 40
```
