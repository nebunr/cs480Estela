#ifndef KEYHANDLER_H
#define KEYHANDLER_H

class KeyHandler{
	public:
		KeyHandler();
		~KeyHandler();
		void Press(char key);
		void Unpress(char key);
		bool IsPressed(char key);
	private:
		bool keys[256];
};

#endif //KEYHANDLER_H
