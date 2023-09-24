/*
   The MIT License (MIT)

   Copyright (c) 2014 Adam Simpson

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
   */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "glfw_utils.h"
#include "renderer.h"
#include "controls.h"
#include "exit_menu_gl.h"

static float screen_scale = 1;

void gamepad_callback(GLFWwindow* window);
void check_user_input(gl_t *state)
{
	// Poll GLFW for key press or mouse input
	glfwPollEvents();
	gamepad_callback(state->window);
}

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}


bool window_should_close(gl_t *state)
{
	if(glfwWindowShouldClose(state->window))
		return true;
	else
		return false;
}

void gamepad_callback(GLFWwindow* window)
{
    // Get render_state from GLFW user pointer
    render_t *render_state = glfwGetWindowUserPointer(window);

    // Define the minimum time (in seconds) that must pass between button presses and significant axis changes
    const double debounce_time = 0.2; // 200 milliseconds

    // Store the time of the last button press
    static double last_button_press_time[GLFW_GAMEPAD_BUTTON_LAST] = {0};

    // Store the accumulated joystick deltas
    static float accum_x = 0;
    static float accum_y = 0;

    // Sensitivity factor for joystick movement
    const float sensitivity = 0.05;

    int joystick = GLFW_JOYSTICK_1;

    // Ensure joystick is present and is a gamepad
    if (glfwJoystickPresent(joystick) && glfwJoystickIsGamepad(joystick)) {
        GLFWgamepadstate state;
        if (glfwGetGamepadState(joystick, &state)) {
            // Let renderer know of activity
            set_activity_time(render_state);

            double current_time = glfwGetTime();

            // Handle button presses with debouncing
            for (int button = 0; button <= GLFW_GAMEPAD_BUTTON_LAST; button++) {
                if (state.buttons[button] == GLFW_PRESS && current_time - last_button_press_time[button] > debounce_time) {
                    last_button_press_time[button] = current_time;

                    switch (button) {
                        case GLFW_GAMEPAD_BUTTON_A:
                            if (render_state->quit_mode) exit_with_selected_program(render_state, window);
                            set_fluid_a(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_B:
                            set_fluid_b(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_X:
                            set_fluid_x(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_Y:
                            set_fluid_y(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_BACK:
                            toggle_dividers(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_START:
                            toggle_pause(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                            move_parameter_up(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                            move_parameter_down(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                            increase_parameter(render_state);
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
                            decrease_parameter(render_state);
                            break;
                    }
                }
            }

            // Match mouse movement to joystick. Invert Y axis for intuitive control
            float dx = sensitivity * state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
            float dy = -sensitivity * state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]; // Y axis is inverted for intuitive control
            accum_x = fmax(fmin(accum_x + dx, 1.0), -1.0); // clamp values between -1 and 1
            accum_y = fmax(fmin(accum_y + dy, 1.0), -1.0); // clamp values between -1 and 1

            set_mover_gl_center(render_state, accum_x, accum_y);
        }
    }
}



static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Get render_state from GLFW user pointer
	render_t *render_state = glfwGetWindowUserPointer(window);

	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{

		// Let renderer know of activity
		set_activity_time(render_state);

		switch(key)
		{ 
			case GLFW_KEY_ESCAPE:
#ifdef EXIT_SIMPLE
				exit_now(render_state, window);
#else
				toggle_quit_mode(render_state);
#endif        
				break;
			case GLFW_KEY_RIGHT:
				increase_parameter(render_state);
				break;
			case GLFW_KEY_LEFT:
				decrease_parameter(render_state);
				break;
			case GLFW_KEY_UP:
				move_parameter_up(render_state);
				break;
			case GLFW_KEY_DOWN:
				move_parameter_down(render_state);
				break;
			case GLFW_KEY_LEFT_BRACKET:
				remove_partition(render_state);
				break;
			case GLFW_KEY_RIGHT_BRACKET:
				add_partition(render_state);
				break;
			case GLFW_KEY_X:
				set_fluid_x(render_state);
				break;
			case GLFW_KEY_Y:
				set_fluid_y(render_state);
				break;
			case GLFW_KEY_A:
				if(render_state->quit_mode)
					exit_with_selected_program(render_state, window);
				set_fluid_a(render_state);
				break;
			case GLFW_KEY_B:
				set_fluid_b(render_state);
				break;
			case GLFW_KEY_D:
				toggle_dividers(render_state);
				break;
			case GLFW_KEY_P:
				toggle_pause(render_state);
				break;
			case GLFW_KEY_L:
				toggle_liquid(render_state);
				break;
			case GLFW_KEY_W:
				increase_points(render_state);//TDH
				break;	
		}
	}
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Get render_state from GLFW user pointer
	render_t *render_state = glfwGetWindowUserPointer(window);

	// Let renderer know of activity
	set_activity_time(render_state);

	float new_x, new_y;
	new_y = (render_state->screen_height*screen_scale - ypos); // Flip y = 0
	new_y = new_y/(0.5*render_state->screen_height*screen_scale) - 1.0;
	new_x = xpos/(0.5*render_state->screen_width*screen_scale) - 1.0;

	set_mover_gl_center(render_state, new_x, new_y);
}

// scroll wheel callback
void wheel_callback(GLFWwindow* window, double x, double y)
{
	// Get render_state from GLFW user pointer
	render_t *render_state = glfwGetWindowUserPointer(window);

	// Let renderer know of activity
	set_activity_time(render_state);    

	// Call increase/decrease mover calls
	if(y > 0.0)
		increase_mover_height(render_state);
	else if(y < 0.0)
		decrease_mover_height(render_state);
	if(x > 0.0)
		increase_mover_width(render_state);
	else if(x < 0.0)
		decrease_mover_width(render_state);
}

// Description: Sets the display, OpenGL context and screen stuff
void init_ogl(gl_t *state, render_t *render_state)
{
	// Set error callback
	glfwSetErrorCallback(error_callback);

	// Initialize GLFW
	if(!glfwInit())
		exit(EXIT_FAILURE);

	// Create window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Try to get full screen
	// Retina screen is a pain...
	GLFWvidmode *mode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
	state->window = glfwCreateWindow(mode->width, mode->height, "SPH", glfwGetPrimaryMonitor(), NULL);

	int window_width, window_height;
	glfwGetWindowSize(state->window, &window_width, &window_height);
	glfwGetFramebufferSize(state->window, &state->screen_width, &state->screen_height);
	glViewport(0, 0, state->screen_width, state->screen_height);

	// Retina screens don't have a 1 to 1 ratio 
	screen_scale = window_width / (float)state->screen_width;

	if(!state->window)
		exit(EXIT_FAILURE);

	// Set current context to window
	glfwMakeContextCurrent(state->window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Set key callback
	glfwSetKeyCallback(state->window, key_callback);

	// Set mouse cursor callback
	glfwSetCursorPosCallback(state->window, mouse_callback);

	// Set scroll wheel callback
	glfwSetScrollCallback(state->window, wheel_callback);

	// Add render state to window pointer
	// Used for key callbacks
	glfwSetWindowUserPointer(state->window, render_state);

	// Disable regular cursor
	glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Disable vsync for true FPS testing
	// Default limit 60 fps
	//    glfwSwapInterval(0);

	// Set background color and clear buffers
	glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT );
}

void swap_ogl(gl_t *state)
{
	glfwSwapBuffers(state->window);

	//    glfwPollEvents();
}

void exit_ogl(gl_t *state)
{
	//    glDeleteProgram(state->shaderProgram);
	//    glDeleteShader(fragmentShader);
	//    glDeleteShader(vertexShader);
	//    glDeleteBuffers(1, &vbo);
	//    glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(state->window);
	glfwTerminate();

	printf("close\n");
}

// Convert pixel coordinates, lower left origin, to gl coordinates, center origin
void pixel_to_gl(gl_t *state, int pixel_x, int pixel_y, float *gl_x, float *gl_y)
{
	float half_x = state->screen_width/2.0;
	float half_y = state->screen_height/2.0;
	*gl_x = pixel_x/half_x - 1.0;
	*gl_y = pixel_y/half_y - 1.0;

}

// Exit and set return value for specific program if one selected
void exit_with_selected_program(render_t *render_state, GLFWwindow* window)
{
	if(render_state->exit_menu_state->mandelbrot_state->selected) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		render_state->return_value = 10;
	}
	else if (render_state->exit_menu_state->sph_state->selected) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		render_state->return_value = 20;
	}
	else if (render_state->exit_menu_state->terminal_state->selected) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		render_state->return_value = 0;
	}
}

// Exit without using program selector
void exit_now(render_t *render_state, GLFWwindow* window) {
	glfwSetWindowShouldClose(window, GL_TRUE);
	render_state->return_value = 0;
}
