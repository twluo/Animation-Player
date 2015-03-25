////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"
#include <time.h>
#define WINDOWTITLE	"Spinning Cube"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc, argv);
	glutMainLoop();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}
static int index = 0;
static float newTime = -4;
static float oldTime = 0;
static bool animate = false;
static Vector3 wind;
static time_t prevTime = time(NULL);
static time_t currentTime = time(NULL);
//static std::vector<GLUI_Spinner*> spinnerList;
////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;
	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow(WindowHandle);
	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float shininess[] = { 100.0 };
	float altSpecular[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 1.0, 0.0, 0.0, 0.0 };
	GLfloat altlight_position[] = { -1.0, 0.0, 0.0, 0.0 };
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, altlight_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, altSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	//Tweaking stuff
	float mass = 1;
	Vector3 start = Vector3(-4, 4, -5);
	Vector3 end = Vector3(4, -4, -3);
	wind = Vector3(0, 0, -10);
	int rowSize = 20;
	int colSize = 20;
	float rowOffset = (end.x - start.x) / rowSize;
	float colOffset = (start.y - end.y) / colSize;
	printf("%f %f\n", rowOffset, colOffset);
	//End of tweaking

	if (argc == 1) {
		skel = new Skeleton("wasp.skel");
		skn = new skin("wasp.skin", skel->worldMatrixes);
		/*
		myCloth = new cloth();
		myCloth->setSpringConstant(1000);
		myCloth->setDampingFactor(10);
		myCloth->setDensity(1.2);
		myCloth->setDrag(0.4);
		myCloth->setWind(wind);
		std::vector<particle*> temp;
		for (int i = 0; i < colSize; i++) {
			temp.clear();
			for (int j = 0; j < rowSize; j++) {
				if (i == 0 && j == 0)
					temp.push_back(new particle(Vector3(start.x + rowOffset *j, start.y - colOffset * i, start.z), mass, true));
				else if (i == 0 && j == rowSize - 1)
					temp.push_back(new particle(Vector3(start.x + rowOffset *j, start.y - colOffset * i, start.z), mass, true));
				else
					temp.push_back(new particle(Vector3(start.x + rowOffset *j, start.y - colOffset * i, start.z), mass, false));
				//printf("Adding %f %f %f\n", start.x + rowOffset *j, start.y - colOffset * i, 0.0);
			}
			//printf("\n");
			myCloth->particles.push_back(temp);
		}
		myCloth->setTriangles();
		myCloth->setSpringDampers();
		*/
		anmtn = new animationClip("wasp_walk.anim", skel->joints);
	}
	else {
		skel = new Skeleton(argv[1]);
		skn = new skin(argv[2], skel->worldMatrixes);
	}


	// Background color
	glClearColor( 0., 0., 0., 1. );
	std::cout << "Editing " << skel->joints[index]->getName() << "'s " << skel->joints[index]->getTag() << std::endl;
	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );
	/*
	glui = GLUI_Master.create_glui("GLUI", 0);
	for (int i = 0; i < skel->joints.size(); i++) {
		GLUI_Spinner *dofSpinner;
		spinnerList.push_back(dofSpinner);
	}
	for (int i = 0; i < skel->joints.size(); i++) {
		if (skel->joints[i]->getTag().compare("x") == 0) {
			std::string name = skel->joints[i]->getName();
			name.append(" ");
			name.append(skel->joints[i]->getTag());
			char *altname = &name[0];
			spinnerList[i] = glui->add_spinner(altname, GLUI_SPINNER_FLOAT);
			spinnerList[i]->set_float_val(skel->joints[i]->getValue());
		}
	}
	glui->add_column(true);
	for (int i = 0; i < skel->joints.size(); i++) {
		if (skel->joints[i]->getTag().compare("y") == 0) {
			std::string name = skel->joints[i]->getName();
			name.append(" ");
			name.append(skel->joints[i]->getTag());
			char *altname = &name[0];
			spinnerList[i] = glui->add_spinner(altname, GLUI_SPINNER_FLOAT);
			spinnerList[i]->set_float_val(skel->joints[i]->getValue());
		}
	}
	glui->add_column(true);
	for (int i = 0; i < skel->joints.size(); i++) {
		if (skel->joints[i]->getTag().compare("z") == 0) {
			std::string name = skel->joints[i]->getName();
			name.append(" ");
			name.append(skel->joints[i]->getTag());
			char *altname = &name[0];
			spinnerList[i] = glui->add_spinner(altname, GLUI_SPINNER_FLOAT);
			spinnerList[i]->set_float_val(skel->joints[i]->getValue());
		}
	}
	*/
	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	//Cube.Update();
	Matrix34 id;
	anmtn->evaluate(newTime);
	if (animate){
		float timeDiff = currentTime - prevTime;
		prevTime = currentTime;
		currentTime = time(NULL);
		newTime = newTime + 0.01;
	}
	//std::cout << time << std::endl;
	skel->calculate(id.IDENTITY);
	skn->update();
	// Tell glut to re-display the scene
	//myCloth->Update(0.01);
	//myCloth->setWind(wind);
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
	
	//for (int i = 0; i < skel->joints.size(); i++) {
	//	spinnerList[i]->set_float_val(skel->joints[i]->getValue());
	//} 
	
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
	index = 0;
	std::cout << "Editing " << skel->joints[index]->getName() << "'s " << skel->joints[index]->getTag() << std::endl;
	//Cube.Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw components
	/*
	Matrix34 id;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(id);
	glBegin(GL_QUADS);
	glColor3d(0, 1, 0);
	glNormal3f(0, 1, 0);
	glVertex3f(-20, -10, -20);
	glVertex3f(-20, -10, 20);
	glVertex3f(20, -10, 20);
	glVertex3f(20, -10, -20);
	glEnd();
	*/
	//myCloth->Draw();
	Cam.Draw();		// Sets up projection & viewing matrices
	//Matrix34 id;
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(id);
	//glBegin(GL_QUADS);
	//glColor3d(1, 0, 0);
	//glVertex3f(2,  - 2,0);
	//glVertex3f(2, + 2, 0);
	//glVertex3f(- 2, +2,0);
	//glVertex3f(- 2, -2, 0);
	//glEnd();
	skel->draw();
	skn->draw();
	//Cube.Draw();

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
			/*
		case 'z':
			wind.x += 0.1;
			wind.Print("New Wind:");
			break;
		case 'Z':
			wind.x -= 0.1;
			wind.Print("New Wind:");
			break;
		case 'x':
			wind.y += 0.1;
			wind.Print("New Wind:");
			break;
		case 'X':
			wind.y -= 0.1;
			wind.Print("New Wind:");
			break;
		case 'c':
			wind.z += 0.1;
			wind.Print("New Wind:");
			break;
		case 'C':
			wind.z -= 0.1;
			wind.Print("New Wind:");
			break;
		case 'w':
			for (int i = 0; i < myCloth->particles.size(); i++) {
				myCloth->particles[0][i]->moveUp();
			}
			break;
		case 's':
			for (int i = 0; i < myCloth->particles.size(); i++) {
				myCloth->particles[0][i]->moveDown();
			}
			break;
		case 'a':
			for (int i = 0; i < myCloth->particles.size(); i++) {
				myCloth->particles[0][i]->moveLeft();
			}
			break;
		case 'd':
			for (int i = 0; i < myCloth->particles.size(); i++) {
				myCloth->particles[0][i]->moveRight();
			}
			break;
		case 'q':
			for (int i = 0; i < myCloth->particles.size(); i++) {
				myCloth->particles[0][i]->moveForward();
			}
			break;
		case 'e':
			for (int i = 0; i < myCloth->particles.size(); i++) {
				myCloth->particles[0][i]->moveBackward();
			}
			break;
			*/
		case 'r':
			Reset();
			skel->reset();
			newTime = -4;
			break;
		case 'l':
			index++;
			if (index == skel->joints.size())
				index = skel->joints.size() - 1;
			std::cout << "Editing " << skel->joints[index]->getName() << "'s " << skel->joints[index]->getTag() << std::endl;
			break;
		case 'j':
			index--;
			if (index < 0)
				index = 0;
			std::cout << "Editing " << skel->joints[index]->getName() << "'s " << skel->joints[index]->getTag() << std::endl;
			break;
		case 'i':
			skel->joints[index]->setValue(skel->joints[index]->getValue() + 0.1);
			break;
		case 'k':
			skel->joints[index]->setValue(skel->joints[index]->getValue() - 0.1);
			break;
		case 'p':
			skel->print();
			break;
		case 't':
			animate = !animate;
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
