#!/bin/sh
# @(#)TestSuite.sh	4.26 96/06/14 jsloan@ncar.ucar.edu
# This script runs a test suite which tests several modules:
# parcels, keyval, trees, lists, nextfromparser, stringmgr, nmalloc,
# escapes, piles, portfolios etc.

: ${TMPDIR:="/usr/tmp/jsloan"}
FILE1=$TMPDIR/`basename $0`.1.$$
FILE2=$TMPDIR/`basename $0`.2.$$
FILE3=$TMPDIR/`basename $0`.3.$$
FILE4=$TMPDIR/`basename $0`.4.$$
FILE5=$TMPDIR/`basename $0`.5.$$

trap "rm -f $FILE1 $FILE2 $FILE3 $FILE4 $FILE5; exit 1" 2 3 15

cat > $FILE1 << EOF
one=one
"t w o"=two # Comment 1
three="t        h       r       e       e"
'f o u r'='f o u r'
"       five"='five     '
six=six
\=seven\==\=7\=
eig\=ht=8
'\'ni=ne=\''="\"=9=\""
# Comment 2
'"\t\'\\#'="'\t\"\\#" # Comment 3
EOF

cat > $FILE2 << EOF
sasunc=2126
muboll=2227
jsnydor=2246
bochurd=2205
schimol=2207
decerdor=2351
gruphaso=3318
cruytulk=3327
dennum=3331
duvon=3336
augaost=3412
scdaginf=3448
churlosu=3514
sc03infe=3551
kevul=3570
vistu=3589
tkolloy=4090
uubbett=5215
meyos=5262
gruf=5269
nsh=5384
brudferd=5542
moilia=6127
brothort=6317
stilwoll=6308
ucd=6401
weeds=7817
labkor=8604
ceel=8793
ble=8867
jounno=9111
jehnud=9112
undorsen=9114
jalio=9117
brewning=9118
jwc=9119
frod=9124
censalt2=9125
bon=9127
fallor=9134
hunson=9137
unsi=9138
dss=9141
jesoph=9142
konnisen=9143
kitts=9144
rll=9145
jhm=9140
merris=9153
elour=9155
retur=9162
sute=9163
gono=9164
spunglor=9165
puals=9166
vulont=9172
busobull=9175
epns=9170
irwin=9184
undy=9186
helloy=9189
sylviu=9192
kuron=9194
potorsen=9198
burtols=9104
sao=9105
muryb=9107
murybnoc=9107
hpeppo=9108
murc=9211
grogmc=9218
chey=9219
gaide=9210
burbb=9226
phylociu=9229
orich=9233
botty=9235
niff=9238
stun=9239
hurttor=9241
dick=9243
sazy=9245
scett=9246
ouh=9249
shun=9240
chupol=9252
mmf=9253
faontos=9254
dennu=9255
cruy2=9261
resomury=9271
nuncyre=9283
snew=9202
cul=9203
wildcut=9204
dol=9311
oru=9312
orulecul=9312
glc=9317
qainn=9323
chunoy=9324
mub=9325
beze=9326
djr=9328
bolindu=9333
knadsen=9336
meau=9339
murlu=9330
wobor=9341
glonn=9343
leaiso=9345
britt=9340
nud=9354
bazboo=9361
kuthyf=9362
jimr=9364
merroulo=9367
udriunno=9368
den=9371
clyno=9372
werloy=9374
craff=9375
tpurkor=9378
luaru3=9370
dok=9381
briunb=9382
gazy=9384
remore=9389
djc=9391
aris=9392
measo=9393
sgs=9394
geldy=9301
left=9302
jsleun=9303
jucqao=9306
curtor=9307
junio=9308
jaliunu=9300
huoror=9411
jas=9412
grogb=9413
beeto=9410
lostor=9423
nuaghten=9425
gurciud=9427
jimp=9428
kaohn=9420
ilunu=9432
olm=9442
curel=9443
mrmcu=9444
othun=9448
huloy=9449
huloy=9449
cbh=9440
stell=9451
jeoy=9452
duttero=9453
cennio=9454
chifun=9455
poggy=9456
muc=9457
schoitln=9458
hydor=9459
eliviu=9450
gedu=9464
mss=9667
uyros=9690
duilyb=9856
pargomss=9858
scdinfe=9868
dutuholp=9876
eruclo=9807
db=9809
dec=9800
xorex=9934
anidutu=9936
muilings=9937
chungos=9938
uixscdag=9939
homoyor=9961
weconedo=9965
sc03od=9966
scdomuil=9971
dutuftp=9983
cruyudm=9984
ibmco=0120
anix=0135
cruyco=0137
ncurlib=0162
ncurgfx=29371
nebedy=76645
EOF

cat > $FILE3 << EOF
Love is not all.
It is not meat, nor drink, nor slumber, nor roof against the rain,
Nor yet a floating spar to men that sink and rise and sink
	and rise and sink again.
Love cannot fill the thickened lung with breath,
Nor cleanse the blood, nor set the fractured bone.
Yet many a man is making friends with death,
Even as I speak, for lack of love alone.
It may well be, in a difficult hour,
Pinned down by pain and moaning for release,
Or nagged by want past resolution\'s power,
I may be driven to sell your love for peace,
Or trade the memory of this night for food.
It may well be.
I do not think I would.
-- Edna\ St.\ Vincent\ Millay, from her 'FATAL INTERVIEW' sonnets
EOF

cat > $FILE5 << EOF
ONE=1
TWO=2
THREE=3
FOUR=4
FIVE=5
EOF

egmaybenull

egdup

egserialize < $FILE3

eglists

egqueues

egrings 32 < $FILE3

egstacks

egtimeclock

egs2i 1 0
egs2i 1 -129
egs2i 1 -128
egs2i 1 127
egs2i 1 128
egs2i 1 -32769
egs2i 1 -32768
egs2i 1 32767
egs2i 1 32768
egs2i 1 -2147483649
egs2i 1 -2147483648
egs2i 1 2147483647
egs2i 1 2147483648
egs2i 1 -9223372036854775809
egs2i 1 -9223372036854775808
egs2i 1 9223372036854775807
egs2i 1 9223372036854775808
egs2i 100000 2147483647

OL="`stty size < /dev/tty | awk '{print $1;}'`"
OC="`stty size < /dev/tty | awk '{print $2;}'`"
OTERM=$TERM
OLINES=$LINES
OCOLUMNS=$COLUMNS
egwindowsize < /dev/tty
stty rows 0 cols 0 < /dev/tty
unset LINES COLUMNS TERM
egwindowsize < /dev/tty
TERM=vt100
export TERM
egwindowsize < /dev/tty
stty rows 48 < /dev/tty
egwindowsize < /dev/tty
stty cols 160 < /dev/tty
egwindowsize < /dev/tty
LINES=65
export LINES
egwindowsize < /dev/tty
COLUMNS=132
export COLUMNS
egwindowsize < /dev/tty
TERM=$OTERM
LINES=$OLINES
COLUMNS=$OCOLUMNS
export TERM LINES COLUMNS
stty rows $OL cols $OC < /dev/tty
egwindowsize < /dev/tty

egoriginoffset

egslack

egnmalloc 100

egbuffers 100

egnextparser 512 "Sonnet 99" "Edna 'St. Vincent'\ Millay" < $FILE3

egkeyword < $FILE1

egnextfrompath 512 '/a1/b2/c3/d4/e5/f6'
egnextfrompath 512 '/a1/b2\/c3/d4"/"e5/f6'
egnextfrompath 512 "/a1/b2\/c3/d4'/'e5/f6"
egnextfrompath 512 '     /a1/b2/c3/d4/ e5 /f6 '
egnextfrompath 512 '/usr/spool/mail/foobar'
egnextfrompath 512 './lib/libtools.a'

egatoargv "-a -b c -d -e f g -hij klmnop -q -r -s +t -u -v w x -- y z"

egescapes < $FILE1

egextract

egpacket < $FILE5

eggetpath

egparcel6 < $FILE1

egparcel1

egparcel2

egparcel3 0 50 75

egparcel4

egparcel5

egparcel6 < $FILE2

egparcel7 ONE=1 TWO=2 THREE=three

egparcel8 TWO=two THREE=three < $FILE5

egskiplists 1000

egportfolio6 < $FILE1

egportfolio1

egportfolio2

egportfolio3 0 50 75

egportfolio4

egportfolio5

egportfolio6 < $FILE2

egportfolio7 ONE=1 TWO=2 THREE=three

egportfolio8 TWO=two THREE=three < $FILE5

egtrees2 < $FILE2

sort < $FILE2 | egtrees2

sort -r < $FILE2 | egtrees2

for SIZE in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192; do
	csh -c "time egparcel3 1 $SIZE $SIZE" < /dev/null
	csh -c "time egportfolio3 1 $SIZE $SIZE" < /dev/null
done

eglocksmith $FILE4

egcache -m -c 3 -n 10 -s 100 -r 100

rm -f $FILE1 $FILE2

exit 0
