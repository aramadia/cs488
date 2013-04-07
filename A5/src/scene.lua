-- An sample array

mat1 = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25)

scene_root = gr.node('root')
scene_root:rotate('X', 45)

for x = -3,3 do
	for y = -3,3 do
		s1 = gr.nh_sphere('s1', {x * 75, y * 75, -400}, 20)
		scene_root:add_child(s1)
		s1:set_material(mat1)		

		b1 = gr.nh_box('b1', {x * 75, y * 75, 0}, 20)
		scene_root:add_child(b1)
		b1:set_material(mat3)

 	end
end



white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

-- node, filename, w, h
-- eye, view, up, fov
-- ambient, lights

-- fov default = 50
gr.render(scene_root, 'fast.png', 400, 400, 
	  {0, 0, 800}, {3, 2, -5}, {0, 1, 0}, 120,
	  {0.3, 0.3, 0.3}, {white_light, orange_light})
