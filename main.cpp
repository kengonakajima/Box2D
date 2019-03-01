
#include "Box2D/Box2D.h"

#include <stdio.h>
#include "moyai/cumino.h"
#include "moyai/client.h"

GLFWwindow *g_window;
const int SCRW=640, SCRH=640;

Keyboard *g_keyboard;
Mouse *g_mouse;
double g_last_mouse_x,g_last_mouse_y;
double g_wheel_yofs;

MoyaiClient *g_moyai_client;

// http://nu-pan.hatenablog.com/entry/20130916/1379339117
class DebugDrawer : public b2Draw{
public:
	DebugDrawer(){
        
    }
	virtual ~DebugDrawer(){}

    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);
		for (int32 i = 0; i < vertexCount; ++i)
		{
			glVertex2f(vertices[i].x, vertices[i].y);
		}
		glEnd();
        
	}

	/// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		glBegin(GL_TRIANGLE_FAN);
		for (int32 i = 0; i < vertexCount; ++i)
		{
			glVertex2f(vertices[i].x, vertices[i].y);
		}
		glEnd();
		glDisable(GL_BLEND);

		glColor4f(color.r, color.g, color.b, 1.0f);
		glBegin(GL_LINE_LOOP);
		for (int32 i = 0; i < vertexCount; ++i)
		{
			glVertex2f(vertices[i].x, vertices[i].y);
		}
		glEnd();
	}


	/// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color){
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 theta = 0.0f;
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x, v.y);
			theta += k_increment;
		}
		glEnd();
        
	}
	
	/// Draw a solid circle.
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color){
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 theta = 0.0f;
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		glBegin(GL_TRIANGLE_FAN);
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x, v.y);
			theta += k_increment;
		}
		glEnd();
		glDisable(GL_BLEND);

		theta = 0.0f;
		glColor4f(color.r, color.g, color.b, 1.0f);
		glBegin(GL_LINE_LOOP);
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x, v.y);
			theta += k_increment;
		}
		glEnd();

		b2Vec2 p = center + radius * axis;
		glBegin(GL_LINES);
		glVertex2f(center.x, center.y);
		glVertex2f(p.x, p.y);
		glEnd();
        
	}

	/// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color){
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINES);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glEnd();
	}

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf){
		b2Vec2 p1 = xf.p, p2;
		const float32 k_axisScale = 0.4f;
		glBegin(GL_LINES);
	
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(p1.x, p1.y);
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		glVertex2f(p2.x, p2.y);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(p1.x, p1.y);
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		glVertex2f(p2.x, p2.y);

		glEnd();
	}
    virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
        print("point: %f %f ",p.x,p.y);        
    }

    
};



void glfw_error_cb( int code, const char *desc ) {
    print("glfw_error_cb. code:%d desc:'%s'", code, desc );
}
void fbsizeCallback( GLFWwindow *window, int w, int h ) {
    print("fbsizeCallback: %d,%d",w,h);
}
void winclose_callback(GLFWwindow *w) {
    exit(0);    
}
void keyboardCallback( GLFWwindow *window, int key, int scancode, int action, int mods ) {
    //    print("keyboardCallback: %d %d %d %d",key,scancode,action,mods);
    g_keyboard->update( key, action, mods & GLFW_MOD_SHIFT, mods & GLFW_MOD_CONTROL, mods & GLFW_MOD_ALT );
    switch(key) {
    case GLFW_KEY_ENTER:
        break;
    }
}
void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods ) {
    //    print("mousebutton: %d %d %d",button,action,mods);
    g_mouse->updateButton( button, action, mods & GLFW_MOD_SHIFT, mods & GLFW_MOD_CONTROL, mods & GLFW_MOD_ALT );
}
void cursorPosCallback( GLFWwindow *window, double x, double y ) {
    g_last_mouse_x+=x-SCRW/2;
    g_last_mouse_y+=y-SCRH/2;
    if(g_last_mouse_x<0) g_last_mouse_x=0; 
    if(g_last_mouse_x>=SCRW)g_last_mouse_x=SCRW;
    if(g_last_mouse_y<0)g_last_mouse_y=0;
    if(g_last_mouse_y>=SCRH)g_last_mouse_y=SCRH;
}
void scrollCallback( GLFWwindow *window, double xofs, double yofs) {
    //    print("scrollcallback: %f %f",xofs, yofs);
    g_wheel_yofs=yofs;
}

    
// This is a simple example of building and running a simulation
// using Box2D. Here we create a large ground box and a small dynamic
// box.
// There are no graphics for this example. Box2D is meant to be used
// with your rendering engine in your game engine.
int main(int argc, char** argv)
{
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

    if( !glfwInit() ) {
        print("can't init glfw");
        exit(1);
    }
    glfwSetErrorCallback( glfw_error_cb );
    g_window =  glfwCreateWindow( SCRW, SCRH, "Space Miner Factory", NULL, NULL );
    if(g_window == NULL ) {
        print("can't open glfw window");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(g_window);
    //    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwSetWindowCloseCallback( g_window, winclose_callback );
    //    glfwSetInputMode( g_window, GLFW_STICKY_KEYS, GL_TRUE );
    glfwSwapInterval(1); // set 1 to use vsync.

    glClearColor(0.2,0.2,0.2,1);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // wireframe 2D is not supported

	glViewport(0, 0, SCRW*2, SCRH*2);
    float scl=0.2;
    glOrtho(-SCRW/2.f*scl, SCRW/2.f*scl, -SCRH/2.f*scl, SCRH/2.f*scl, 1.f, -1.f);
    
    // controls
    g_keyboard = new Keyboard();
    glfwSetKeyCallback( g_window, keyboardCallback );
    g_mouse = new Mouse();
    glfwSetMouseButtonCallback( g_window, mouseButtonCallback );
    glfwSetCursorPosCallback( g_window, cursorPosCallback );
    glfwSetScrollCallback( g_window, scrollCallback );

    glfwSetFramebufferSizeCallback( g_window, fbsizeCallback );
    
    g_moyai_client = new MoyaiClient(g_window, SCRW, SCRH );



    ///////////

    
	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);
    DebugDrawer dd;
    dd.SetFlags(0x1f);
    world.SetDebugDraw(&dd);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox(50.0f, 5.0f);
    
	// Add the ground fixture to the ground body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

    //ball
    b2CircleShape ballShape;
    ballShape.m_p.Set(2.0f,3.0f);
    ballShape.m_radius=2.0f;

    b2FixtureDef ballFDef;
    ballFDef.shape = &ballShape;
    ballFDef.density=1.0f;
    ballFDef.friction=0.3f;
    b2BodyDef ballBDef;
    ballBDef.type = b2_dynamicBody;
    ballBDef.position.Set(1.0f,20.0f);
    b2Body *ballBody = world.CreateBody(&ballBDef);
    ballBody->CreateFixture(&ballFDef);

    
    b2Vec2 verts1[3];
    verts1[0].Set(0.0f, 0.0f);
    verts1[1].Set(4.0f, 0.0f);
    verts1[2].Set(0.0f, 4.0f);
    b2PolygonShape p1;
    p1.Set(verts1, 3);

    b2Vec2 verts2[4];
    verts2[0].Set(0.0f, 0.0f);
    verts2[1].Set(2.0f, 0.0f);
    verts2[2].Set(2.0f, 3.0f);    
    verts2[3].Set(0.0f, 1.5f);
    b2PolygonShape p2;
    p2.Set(verts2, 4);
    
        
    const int n=30;
    for(int i=0;i<n;i++) {
        // poly stone
        b2FixtureDef stoneFDef;
        if(range(0,1)<0.5) {
            stoneFDef.shape = &p1;
        } else {
            stoneFDef.shape = &p2;
        }
        stoneFDef.density=2.0f;
        stoneFDef.friction=0.5f;
        b2BodyDef stoneBDef;
        stoneBDef.type = b2_dynamicBody;
        stoneBDef.position.Set(range(-5,5), range(10,30));
        b2Body *stoneBody = world.CreateBody(&stoneBDef);
        stoneBody->CreateFixture(&stoneFDef);
    }

    
	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

    int loopcnt=0;
	// This is our little game loop.
    while( !glfwWindowShouldClose(g_window) ) {
        loopcnt++;
        glfwPollEvents();

        if(loopcnt%37==0) {
            b2FixtureDef ballFDef;
            ballFDef.shape = &ballShape;
            ballFDef.density=1.0f;
            ballFDef.friction=0.3f;
            b2BodyDef ballBDef;
            ballBDef.type = b2_dynamicBody;
            ballBDef.position.Set(range(-5,5), range(40,50));            
            b2Body *ballBody = world.CreateBody(&ballBDef);
            ballBody->CreateFixture(&ballFDef);
        }

        if(loopcnt%20==0) {
            // poly stone
            b2FixtureDef stoneFDef;
            if(range(0,1)<0.5) {
                stoneFDef.shape = &p1;
            } else {
                stoneFDef.shape = &p2;
            }
            stoneFDef.density=2.0f;
            stoneFDef.friction=0.5f;
            b2BodyDef stoneBDef;
            stoneBDef.type = b2_dynamicBody;
            stoneBDef.position.Set(range(-5,5), range(40,50));            
            b2Body *stoneBody = world.CreateBody(&stoneBDef);
            stoneBody->CreateFixture(&stoneFDef);
        }
        
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

        //		printf("box %4.2f %4.2f %4.2f\n", position.x, position.y, angle);

        glClear(GL_COLOR_BUFFER_BIT);

        world.DrawDebugData();

        glfwSwapBuffers(g_window);
        glFlush();
	}

	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.

	return 0;
}
