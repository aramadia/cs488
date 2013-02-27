--
-- CS488 -- Introduction to Computer Graphics
-- 
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

orange = gr.material({1.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0.0, 0.0, 0.0}, {1.0, 1.0,1.0}, 30)

torso = gr.node('torso')
torso:translate(0.0,2.0,0.0)
	torsoP = gr.sphere('torsoP')
	torsoP:set_material(white)
	torsoP:scale(2.25, 3.0, 1.0)
torso:add_child(torsoP)
rootnode:add_child(torso)

shoulder =gr.node("shoulder")
shoulder:translate(0.0, 2.75, 0.0)
	shoulderP = gr.sphere('shoulderP')
	shoulderP:scale(3.5, 0.5, 1.0)
	shoulderP:set_material(green)

shoulder:add_child(shoulderP)

torso:add_child(shoulder)

torsoJoint = gr.joint('torsoJoint', {-15.0, 0.0, 15.0}, {0.0,0.0,0.0})
torsoJoint:translate(0.0, 0.75, 0.0)
shoulder:add_child(torsoJoint)




neck = gr.node('neck')

neck:translate(0.0, 0.25, 0.0)
	neckP = gr.sphere('neckP')
	neckP:set_material(blue)
	neckP:scale(0.5, 0.75, 0.5)
neck:add_child(neckP)
torsoJoint:add_child(neck)

neckJoint = gr.joint('neckJoint', {-20.0, 0.0, 20.0}, {-90.0, 0.0, 90.0})
neckJoint:translate(0.0, .75, 0.0)
neck:add_child(neckJoint)

head = gr.node('head')
head:translate(0.0, .75, 0.0)
	headP = gr.sphere('headP')
	headP:set_material(white)
	headP:scale(1.0, 1.5, 1.0)
	nose = gr.sphere('gr')
	nose:set_material(red)

	nose:translate(0.0, 0.0, 1.0)
	nose:scale(0.25, 0.25, 0.25)

head:add_child(headP)
head:add_child(nose)
neckJoint:add_child(head)

-- MIRROR THIS LEFT ARM

shoulderLJoint = gr.joint('shoulderLJoint', {-90.0, 0.0, 90.0}, {-5.0, 0.0, 5.0})
shoulderLJoint:translate(-2.75, 0.0, 0.0)
shoulder:add_child(shoulderLJoint)


upperArmL = gr.node('upperArmL')
upperArmL:translate(0.0, -2.0, 0.0)
	upperArmLP = gr.sphere('upperArmLP')
	upperArmLP:set_material(white)
	upperArmLP:scale(0.4, 2.25, 1.0)
upperArmL:add_child(upperArmLP)
shoulderLJoint:add_child(upperArmL)

elbowL = gr.joint('elbowL', {0.0, 0.0, 90.0}, {-5.0, 0.0, 5.0})
elbowL:translate(0.0, -1.125, 0.0)
upperArmL:add_child(elbowL)

upperForearmL = gr.node('upperForearmL')
upperForearmL:translate(0.0, -1.125, 0.0)
	upperForearmLP = gr.sphere('upperForearmLP')
	upperForearmLP:set_material(orange)
	upperForearmLP:scale(0.4, 2.25, 1.0)
upperForearmL:add_child(upperForearmLP)
elbowL:add_child(upperForearmL)

wristL =gr.joint('wristL', {-30.0,0.0,30.0}, {-5.0, 0.0, 5.0})
wristL:translate(0.0, -2.25, 0.0)
upperForearmL:add_child(wristL)

handL = gr.node('handL')
handL:translate(0.0, -0.625, 0.0)
	handLP = gr.sphere('handLP')
	handLP:scale(0.4,1.25,0.75)
handL:add_child(handLP)
wristL:add_child(handL)
-- END LEFT ARM

-- RIGHT ARM

shoulderRJoint = gr.joint('shoulderRJoint', {-90.0, 0.0, 90.0}, {-5.0, 0.0, 5.0})
shoulderRJoint:translate(2.75, 0.0, 0.0)
shoulder:add_child(shoulderRJoint)


upperArmR = gr.node('upperArmR')
upperArmR:translate(0.0, -2.0, 0.0)
	upperArmRP = gr.sphere('upperArmRP')
	upperArmRP:set_material(white)
	upperArmRP:scale(0.4, 2.25, 1.0)
upperArmR:add_child(upperArmRP)
shoulderRJoint:add_child(upperArmR)

elbowR = gr.joint('elbowR', {0.0, 0.0, 90.0}, {-5.0, 0.0, 5.0})
elbowR:translate(0.0, -1.125, 0.0)
upperArmR:add_child(elbowR)

upperForearmR = gr.node('upperForearmR')
upperForearmR:translate(0.0, -1.125, 0.0)
	upperForearmRP = gr.sphere('upperForearmRP')
	upperForearmRP:set_material(orange)
	upperForearmRP:scale(0.4, 2.25, 1.0)
upperForearmR:add_child(upperForearmRP)
elbowR:add_child(upperForearmR)

wristR =gr.joint('wristR', {-30.0,0.0,30.0}, {-5.0, 0.0, 5.0})
wristR:translate(0.0, -2.25, 0.0)
upperForearmR:add_child(wristR)

handR = gr.node('handR')
handR:translate(0.0, -0.625, 0.0)
	handRP = gr.sphere('handRP')
	handRP:scale(0.4,1.25,0.75)
handR:add_child(handRP)
wristR:add_child(handR)
-- END RIGHT ARM

hip = gr.node('hip')
hip:translate(0.0, -2.0, 0.0)
	hipP = gr.sphere('hipP')
	hipP:set_material(black)
	hipP:scale(2.5, 1.0, 1.0)
hip:add_child(hipP)
torso:add_child(hip)

-- DA LEFT LEG
hipSocketL = gr.joint('hipSocketL', {-5.0, 0.0, 90.0}, {0.0,0.0,90.0})
hipSocketL:translate(-1.75, 0.0,0.0)
hip:add_child(hipSocketL)



thighL = gr.node('thighL')
thighL:translate(0.0, -2.0, 0.0)
	thighLP = gr.sphere('thighLP')
	thighLP:set_material(white)
	thighLP:scale(0.75, 2.25, 1.0)
thighL:add_child(thighLP)
hipSocketL:add_child(thighL)

kneeL = gr.joint('kneeL', {0.0, 0.0, 90.0}, {-5.0, 0.0, 5.0})
kneeL:translate(0.0, -1.125, 0.0)
thighL:add_child(kneeL)

calfL = gr.node('calfL')
calfL:translate(0.0, -2.125, 0.0)
	calfLP = gr.sphere('calfLP')
	calfLP:set_material(orange)
	calfLP:scale(0.75, 2.25, 1.0)
calfL:add_child(calfLP)
kneeL:add_child(calfL)

ankleL =gr.joint('ankleL', {-30.0,0.0,30.0}, {-5.0, 0.0, 5.0})
ankleL:translate(0.0, -2.25, 0.0)
calfL:add_child(ankleL)

footL = gr.node('footL')
footL:rotate('x', 90.0)
footL:translate(0.0, -0.625, 0.0)

	footLP = gr.sphere('footLP')
	footLP:scale(0.8,1.25,0.5)
footL:add_child(footLP)
ankleL:add_child(footL)

-- end left leg

-- DA RIGHT LEG
hipSocketR = gr.joint('hipSocketR', {-5.0, 0.0, 90.0}, {0.0,0.0,90.0})
hipSocketR:translate(1.75, 0.0,0.0)
hip:add_child(hipSocketR)



thighR = gr.node('thighR')
thighR:translate(0.0, -2.0, 0.0)
	thighRP = gr.sphere('thighRP')
	thighRP:set_material(white)
	thighRP:scale(0.75, 2.25, 1.0)
thighR:add_child(thighRP)
hipSocketR:add_child(thighR)

kneeR = gr.joint('kneeR', {0.0, 0.0, 90.0}, {-5.0, 0.0, 5.0})
kneeR:translate(0.0, -1.125, 0.0)
thighR:add_child(kneeR)

calfR = gr.node('calfR')
calfR:translate(0.0, -2.125, 0.0)
	calfRP = gr.sphere('calfRP')
	calfRP:set_material(orange)
	calfRP:scale(0.75, 2.25, 1.0)
calfR:add_child(calfRP)
kneeR:add_child(calfR)

ankleR =gr.joint('ankleR', {-30.0,0.0,30.0}, {-5.0, 0.0, 5.0})
ankleR:translate(0.0, -2.25, 0.0)
calfR:add_child(ankleR)

footR = gr.node('footR')
footR:rotate('x', 90.0)
footR:translate(0.0, -0.625, 0.0)

	footRP = gr.sphere('footRP')
	footRP:scale(0.8,1.25,0.5)
footR:add_child(footRP)
ankleR:add_child(footR)

-- end left leg



-- make the model fit in the screen
rootnode:scale(0.20, 0.20, 0.20)
return rootnode
