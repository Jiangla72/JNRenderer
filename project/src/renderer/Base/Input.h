#pragma once
#include<unordered_map>
#include "glm/glm.hpp"
#include "Base/core.h"
class JNAPI Input
{
private:
	static std::unordered_map<int, bool> keyPressEvent;
	static std::unordered_map<int, bool> keyReleaseEvent;
	static std::unordered_map<int, bool> keyHeldEvent;
	static unsigned int mouseReleased;
	static unsigned int mouseHeld;
	static unsigned int mouseClicked;
public:
	static glm::vec2 mousePosition;
	static glm::vec2 mouseMotion;
	static glm::vec2 scrollOffset;
	static bool mouseMoving;
public:
	Input() {};
	~Input() {};
public:
	static bool getKey(int key);
	static bool getPressedKey(int key);
	static bool getReleasedKey(int key);
	static bool getMouseClicked(int mouseButton);
	static bool getMouseReleased(int mouseButton);
	static bool getMouseButton(int mouseButton);

	static void setPressedKey(int key);
	static void setReleasedKey(int key);
	static void setMouseClicked(int mouseButton);
	static void setMouseReleased(int mouseButton);
	static void setMouseMove(float x, float y);
	static void setMouseScroll(float xOffset, float yOffset);
};


#define JN_KEY_SPACE              32
#define JN_KEY_APOSTROPHE         39  /* ' */
#define JN_KEY_COMMA              44  /* , */
#define JN_KEY_MINUS              45  /* - */
#define JN_KEY_PERIOD             46  /* . */
#define JN_KEY_SLASH              47  /* / */
#define JN_KEY_0                  48
#define JN_KEY_1                  49
#define JN_KEY_2                  50
#define JN_KEY_3                  51
#define JN_KEY_4                  52
#define JN_KEY_5                  53
#define JN_KEY_6                  54
#define JN_KEY_7                  55
#define JN_KEY_8                  56
#define JN_KEY_9                  57
#define JN_KEY_SEMICOLON          59  /* ; */
#define JN_KEY_EQUAL              61  /* = */
#define JN_KEY_A                  65
#define JN_KEY_B                  66
#define JN_KEY_C                  67
#define JN_KEY_D                  68
#define JN_KEY_E                  69
#define JN_KEY_F                  70
#define JN_KEY_G                  71
#define JN_KEY_H                  72
#define JN_KEY_I                  73
#define JN_KEY_J                  74
#define JN_KEY_K                  75
#define JN_KEY_L                  76
#define JN_KEY_M                  77
#define JN_KEY_N                  78
#define JN_KEY_O                  79
#define JN_KEY_P                  80
#define JN_KEY_Q                  81
#define JN_KEY_R                  82
#define JN_KEY_S                  83
#define JN_KEY_T                  84
#define JN_KEY_U                  85
#define JN_KEY_V                  86
#define JN_KEY_W                  87
#define JN_KEY_X                  88
#define JN_KEY_Y                  89
#define JN_KEY_Z                  90
#define JN_KEY_LEFT_BRACKET       91  /* [ */
#define JN_KEY_BACKSLASH          92  /* \ */
#define JN_KEY_RIGHT_BRACKET      93  /* ] */
#define JN_KEY_GRAVE_ACCENT       96  /* ` */
#define JN_KEY_WORLD_1            161 /* non-US #1 */
#define JN_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define JN_KEY_ESCAPE             256
#define JN_KEY_ENTER              257
#define JN_KEY_TAB                258
#define JN_KEY_BACKSPACE          259
#define JN_KEY_INSERT             260
#define JN_KEY_DELETE             261
#define JN_KEY_RIGHT              262
#define JN_KEY_LEFT               263
#define JN_KEY_DOWN               264
#define JN_KEY_UP                 265
#define JN_KEY_PAGE_UP            266
#define JN_KEY_PAGE_DOWN          267
#define JN_KEY_HOME               268
#define JN_KEY_END                269
#define JN_KEY_CAPS_LOCK          280
#define JN_KEY_SCROLL_LOCK        281
#define JN_KEY_NUM_LOCK           282
#define JN_KEY_PRINT_SCREEN       283
#define JN_KEY_PAUSE              284
#define JN_KEY_F1                 290
#define JN_KEY_F2                 291
#define JN_KEY_F3                 292
#define JN_KEY_F4                 293
#define JN_KEY_F5                 294
#define JN_KEY_F6                 295
#define JN_KEY_F7                 296
#define JN_KEY_F8                 297
#define JN_KEY_F9                 298
#define JN_KEY_F10                299
#define JN_KEY_F11                300
#define JN_KEY_F12                301
#define JN_KEY_F13                302
#define JN_KEY_F14                303
#define JN_KEY_F15                304
#define JN_KEY_F16                305
#define JN_KEY_F17                306
#define JN_KEY_F18                307
#define JN_KEY_F19                308
#define JN_KEY_F20                309
#define JN_KEY_F21                310
#define JN_KEY_F22                311
#define JN_KEY_F23                312
#define JN_KEY_F24                313
#define JN_KEY_F25                314
#define JN_KEY_KP_0               320
#define JN_KEY_KP_1               321
#define JN_KEY_KP_2               322
#define JN_KEY_KP_3               323
#define JN_KEY_KP_4               324
#define JN_KEY_KP_5               325
#define JN_KEY_KP_6               326
#define JN_KEY_KP_7               327
#define JN_KEY_KP_8               328
#define JN_KEY_KP_9               329
#define JN_KEY_KP_DECIMAL         330
#define JN_KEY_KP_DIVIDE          331
#define JN_KEY_KP_MULTIPLY        332
#define JN_KEY_KP_SUBTRACT        333
#define JN_KEY_KP_ADD             334
#define JN_KEY_KP_ENTER           335
#define JN_KEY_KP_EQUAL           336
#define JN_KEY_LEFT_SHIFT         340
#define JN_KEY_LEFT_CONTROL       341
#define JN_KEY_LEFT_ALT           342
#define JN_KEY_LEFT_SUPER         343
#define JN_KEY_RIGHT_SHIFT        344
#define JN_KEY_RIGHT_CONTROL      345
#define JN_KEY_RIGHT_ALT          346
#define JN_KEY_RIGHT_SUPER        347
#define JN_KEY_MENU               348

#define JN_KEY_LAST               JN_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
#define JN_MOD_SHIFT           0x0001
  /*! @brief If this bit is set one or more Control keys were held down.
   *
   *  If this bit is set one or more Control keys were held down.
   */
#define JN_MOD_CONTROL         0x0002
   /*! @brief If this bit is set one or more Alt keys were held down.
	*
	*  If this bit is set one or more Alt keys were held down.
	*/
#define JN_MOD_ALT             0x0004
	/*! @brief If this bit is set one or more Super keys were held down.
	 *
	 *  If this bit is set one or more Super keys were held down.
	 */
#define JN_MOD_SUPER           0x0008
	 /*! @brief If this bit is set the Caps Lock key is enabled.
	  *
	  *  If this bit is set the Caps Lock key is enabled and the @ref
	  *  JN_LOCK_KEY_MODS input mode is set.
	  */
#define JN_MOD_CAPS_LOCK       0x0010
	  /*! @brief If this bit is set the Num Lock key is enabled.
	   *
	   *  If this bit is set the Num Lock key is enabled and the @ref
	   *  JN_LOCK_KEY_MODS input mode is set.
	   */
#define JN_MOD_NUM_LOCK        0x0020

	   /*! @} */

	   /*! @defgroup buttons Mouse buttons
		*  @brief Mouse button IDs.
		*
		*  See [mouse button input](@ref input_mouse_button) for how these are used.
		*
		*  @ingroup input
		*  @{ */
#define JN_MOUSE_BUTTON_1         0
#define JN_MOUSE_BUTTON_2         1
#define JN_MOUSE_BUTTON_3         2
#define JN_MOUSE_BUTTON_4         3
#define JN_MOUSE_BUTTON_5         4
#define JN_MOUSE_BUTTON_6         5
#define JN_MOUSE_BUTTON_7         6
#define JN_MOUSE_BUTTON_8         7
#define JN_MOUSE_BUTTON_LAST      JN_MOUSE_BUTTON_8
#define JN_MOUSE_BUTTON_LEFT      JN_MOUSE_BUTTON_1
#define JN_MOUSE_BUTTON_RIGHT     JN_MOUSE_BUTTON_2
#define JN_MOUSE_BUTTON_MIDDLE    JN_MOUSE_BUTTON_3
		/*! @} */

		/*! @defgroup joysticks Joysticks
		 *  @brief Joystick IDs.
		 *
		 *  See [joystick input](@ref joystick) for how these are used.
		 *
		 *  @ingroup input
		 *  @{ */
#define JN_JOYSTICK_1             0
#define JN_JOYSTICK_2             1
#define JN_JOYSTICK_3             2
#define JN_JOYSTICK_4             3
#define JN_JOYSTICK_5             4
#define JN_JOYSTICK_6             5
#define JN_JOYSTICK_7             6
#define JN_JOYSTICK_8             7
#define JN_JOYSTICK_9             8
#define JN_JOYSTICK_10            9
#define JN_JOYSTICK_11            10
#define JN_JOYSTICK_12            11
#define JN_JOYSTICK_13            12
#define JN_JOYSTICK_14            13
#define JN_JOYSTICK_15            14
#define JN_JOYSTICK_16            15
#define JN_JOYSTICK_LAST          JN_JOYSTICK_16
		 /*! @} */

		 /*! @defgroup gamepad_buttons Gamepad buttons
		  *  @brief Gamepad buttons.
		  *
		  *  See @ref gamepad for how these are used.
		  *
		  *  @ingroup input
		  *  @{ */
#define JN_GAMEPAD_BUTTON_A               0
#define JN_GAMEPAD_BUTTON_B               1
#define JN_GAMEPAD_BUTTON_X               2
#define JN_GAMEPAD_BUTTON_Y               3
#define JN_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define JN_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define JN_GAMEPAD_BUTTON_BACK            6
#define JN_GAMEPAD_BUTTON_START           7
#define JN_GAMEPAD_BUTTON_GUIDE           8
#define JN_GAMEPAD_BUTTON_LEFT_THUMB      9
#define JN_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define JN_GAMEPAD_BUTTON_DPAD_UP         11
#define JN_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define JN_GAMEPAD_BUTTON_DPAD_DOWN       13
#define JN_GAMEPAD_BUTTON_DPAD_LEFT       14
#define JN_GAMEPAD_BUTTON_LAST            JN_GAMEPAD_BUTTON_DPAD_LEFT

#define JN_GAMEPAD_BUTTON_CROSS       JN_GAMEPAD_BUTTON_A
#define JN_GAMEPAD_BUTTON_CIRCLE      JN_GAMEPAD_BUTTON_B
#define JN_GAMEPAD_BUTTON_SQUARE      JN_GAMEPAD_BUTTON_X
#define JN_GAMEPAD_BUTTON_TRIANGLE    JN_GAMEPAD_BUTTON_Y
		  /*! @} */

		  /*! @defgroup gamepad_axes Gamepad axes
		   *  @brief Gamepad axes.
		   *
		   *  See @ref gamepad for how these are used.
		   *
		   *  @ingroup input
		   *  @{ */
#define JN_GAMEPAD_AXIS_LEFT_X        0
#define JN_GAMEPAD_AXIS_LEFT_Y        1
#define JN_GAMEPAD_AXIS_RIGHT_X       2
#define JN_GAMEPAD_AXIS_RIGHT_Y       3
#define JN_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define JN_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define JN_GAMEPAD_AXIS_LAST          JN_GAMEPAD_AXIS_RIGHT_TRIGGER
		   /*! @} */