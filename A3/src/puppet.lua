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

torso = gr.node('torso')
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

neck = gr.node('neck')

neck:translate(0.0, 1.0, 0.0)
	neckP = gr.sphere('neckP')
	neckP:set_material(blue)
	neckP:scale(0.5, 0.75, 0.5)
neck:add_child(neckP)
shoulder:add_child(neck)

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

shoulderLJoint = gr.joint('shoulderLJoint', {-90.0, 0.0, 90.0}, {-5.0, 0.0, 5.0})
shoulderLJoint:translate(-2.75, 0.0, 0.0)
shoulder:add_child(shoulderLJoint)

upperArmL = gr.node('upperArmL')
upperArmL:translate(0.0, -2.5, 0.0)
	upperArmLP = gr.sphere('upperArmLP')
	upperArmLP:set_material(white)
	upperArmLP:scale(0.4, 3.0, 1.0)
upperArmL:add_child(upperArmLP)
shoulderLJoint:add_child(upperArmL)





rootnode:scale(0.20, 0.20, 0.20)
return rootnode
