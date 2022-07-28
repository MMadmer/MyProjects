#include <SFML/Graphics.hpp>

using namespace sf;

const int H = 22;
const int W = 110;
const int height = 400;
const int weight = 600;

float offsetx = 0, offsety = 0;
bool visible = false;
RectangleShape ColAkt(Vector2f(32, 32));
RectangleShape ColFin(Vector2f(32, 32));
bool IsActive = false;
class player;

String Map[H];
String Map_1[H] = {
"C                                                                                                   C         ",
"C                             A                                                                     C         ",
"C                             A                                                                     C         ",
"C      SSSSSLLSSSSLLSSLSSLSSSSS                                                                     C         ",
"C   SSSS   SSSS  SSSSSSSSSS                                                                         C         ",
"C                                                                                                   C         ",
"CS                                                                                                  C         ",
"CS                                                                         ILI  III           2     C         ",
"CSSSSSS                                                              I  IIIIII      II    IIIII     C         ",
"C SSSSS                                        S     I   I    I                                     C         ",
"C     SSSSLLSSSLLSLLSSSLLSSSSSSSS      S    S  S                I  I                                C         ",
"C        SSSSSSSSSSSSSSSSSSSSS         S    S  S                                                    C         ",
"C           S   S  S  S            S   S    S  S                                                    C         ",
"C                                  S   S    S  S                                                    C         ",
"C                               SSSS   S    S  S                                                    C         ",
"C                            SSSSSSS   S    S  S                                                    C         ",
"C                         SSSSSSSSSSLLLSLLLLGLLGLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLGGGGGGGGGG",
"GGGGGGGGGGGGLGGGLGGLGGLGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
};
String Map_2[H] = {
"C                                                                                                   C         ",
"C                                                                                                   C         ",
"C                                                                                                   C         ",
"C                         ISLLSISLSII                                                               C         ",
"CA                      IIISSSS SSS                                                                 C         ",
"CA                                     I                                                            C         ",
"CSSSSLLS           SSS                   I            I                                             C         ",
"C    LL                                    IIIII                                                    C         ",
"C    LL   S     S                                  I                                                C         ",
"C    LL                                                                                             C         ",
"C    LL      S                                                                                      C         ",
"C    LL                                                                                             C         ",
"C    LL        SSSSLSSSS                     SLLLLLLLLLS                                            C         ",
"C    LL            L                         SSSSSSSSSSS                                            C         ",
"C    LL            L      S  S                    S                                                 C         ",
"C    LL            L                              S                                                 C         ",
"C    LL            L           SSLLS              SF                                                C         ",
"GLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLGGGGGGGGGGGGGGGGGGGGGGGGGGGGLLLLLLLLLLLLLLLLGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
};

class player {
public:

	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;
	bool life = true;

	player(Texture &image) {

		sprite.setTexture(image);

		rect = FloatRect(1 * 32, 15 * 32, 43, 67);

		dx = dy = 0;
		currentFrame = 0;
	}

	void update(float time) {
		rect.left += dx * time;

		Collision(0);

		if (!onGround) dy = dy + 0.0005 * time;
		rect.top += dy*time;
		onGround = false;
		Collision(1);

		time *= 0.65;
		 
		currentFrame += 0.007*time;
		if (currentFrame > 9) currentFrame -= 9;
		if (dx == 0) sprite.setTextureRect(IntRect(42, 23, 43, 67));
		if (dx > 0) { 
			switch (int(currentFrame)) {
			case 0:
				sprite.setTextureRect(IntRect(20, 111, 36, 66));
			break;
			case 1:
				sprite.setTextureRect(IntRect(68, 111, 36, 66));
			break;
			case 2:
				sprite.setTextureRect(IntRect(120, 111, 53, 60));
			break;
			case 3:
				sprite.setTextureRect(IntRect(186, 111, 52, 65));
				break;
			case 4:
				sprite.setTextureRect(IntRect(259, 111, 46, 65));
				break;
			case 5:
				sprite.setTextureRect(IntRect(320, 111, 41, 66));
				break;
			case 6:
				sprite.setTextureRect(IntRect(378, 111, 35, 65));
			break;
			case 7:
				sprite.setTextureRect(IntRect(428, 111, 45, 62));
			break;
			case 8:
				sprite.setTextureRect(IntRect(486, 111, 56, 65));
			break;
			case 9:
				sprite.setTextureRect(IntRect(555, 111, 43, 65));
			break;
		}
		}
		if (dx < 0) {
			switch (int(currentFrame)) {
			case 0:
				sprite.setTextureRect(IntRect(20 + 36, 111, -36, 66));
				break;
			case 1:
				sprite.setTextureRect(IntRect(68 + 36, 111, -36, 66));
				break;
			case 2:
				sprite.setTextureRect(IntRect(120 + 53, 111, -53, 60));
				break;
			case 3:
				sprite.setTextureRect(IntRect(186 + 52, 111, -52, 65));
				break;
			case 4:
				sprite.setTextureRect(IntRect(259 + 46, 111, -46, 65));
				break;
			case 5:
				sprite.setTextureRect(IntRect(320 + 41, 111, -41, 66));
				break;
			case 6:
				sprite.setTextureRect(IntRect(378 + 35, 111, -35, 65));
				break;
			case 7:
				sprite.setTextureRect(IntRect(428 + 45, 111, -45, 62));
				break;
			case 8:
				sprite.setTextureRect(IntRect(486 + 56, 111, -56, 65));
				break;
			case 9:
				sprite.setTextureRect(IntRect(555 + 43, 111, -43, 65));
				break;
			}
		}

		sprite.setPosition(rect.left - offsetx, rect.top - offsety);

		dx = 0;
	}

	void choiseMap() {
		if (Keyboard::isKeyPressed(Keyboard::Num1)) {
			for (int i = 0; i < H; i++) Map[i] = Map_1[i];

			IsActive = false;

			offsetx = 0;
			offsety = 0;

			rect = FloatRect(1 * 32, 15 * 32, 43, 67);
			dx = dy = 0;
			currentFrame = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2)) {
			for (int i = 0; i < H; i++) Map[i] = Map_2[i];

			IsActive = false;

			rect = FloatRect(45 * 32, 15 * 32, 43, 67);
			dx = dy = 0;
			currentFrame = 0;
		}
	}

	void Collision(int dir) {
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++) {
				if ((Map[i][j] == 'G') || (Map[i][j] == 'C') || (Map[i][j] == 'S') || (Map[i][j] == 'I')) {
					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height; dy = 0; onGround = true;}
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32; dy = 0; }
				}
				if (Map[i][j] == 'A') {
					visible = true;
					IsActive = true;
					}
				if (Map[i][j] == '2') {
					for (int i = 0; i < H; i++) Map[i] = Map_2[i];

					IsActive = false;

					offsetx = 0;
					offsety = 0;

					rect = FloatRect(45 * 32, 15 * 32, 43, 67);
					dx = dy = 0;
					currentFrame = 0;
				}
				if (Map[i][j] == 'F') life = false;
			for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
					for (int j = rect.left / 32, x = (rect.left + 15) / 32; j < (rect.left + rect.width) / 32, x < (rect.left + rect.width - 15) / 32; j++, x++) {
						if ((i == i) && (j == x) && (Map[i][j] == 'L')) life = false;
					}
			}
	}
};

class Blocks {
public:
	Sprite Block;
	FloatRect rect;

	void set(Texture &image, float frame, int x, int y, int tx1, int ty1, int tx2, int ty2, int tx3, int ty3, int one, int two, int three) {
		Block.setTexture(image);
		if (int(frame) == one) Block.setTextureRect(IntRect(tx1 * 32, ty1 * 32, 32, 32));
		if (int(frame) == two) Block.setTextureRect(IntRect(tx2 * 32, ty2 * 32, 32, 32));
		if (int(frame) == three) Block.setTextureRect(IntRect(tx3 * 32, ty3 * 32, 32, 32));
		Block.setPosition(x * 32 - offsetx, y * 32 - offsety);
	}
	void set(Texture &image, int x, int y, int tx, int ty) {
		Block.setTexture(image);
		Block.setTextureRect(IntRect(tx * 32, ty * 32, 32, 32));
		Block.setPosition(x * 32 - offsetx, y * 32 - offsety);
	}
};

	int main(int &argc, char argv) {
		float textureFrame = 0;

		Clock clock;

		Image image;
		image.loadFromFile("assets/characters/gg.png");
		image.createMaskFromColor(Color::White);
		Texture t;
		t.loadFromImage(image);
		Texture B;
		B.loadFromFile("assets/textures/tileset.png");

		RenderWindow window(VideoMode(weight, height), "Game");

		player p(t);
		Blocks lava, ground, stone, InvStone;

		for (int i = 0; i < H; i++) Map[i] = Map_1[i];

		while ((window.isOpen()) && (!Keyboard::isKeyPressed(Keyboard::Escape)) && (p.life == true)) {
			float time = clock.getElapsedTime().asMicroseconds();
			float textureTime = clock.getElapsedTime().asMicroseconds();
			clock.restart();
			time = time / 500;

			if (time > 20) time = 40;

			textureTime = textureTime / 800;
			textureFrame += 0.007*textureTime;

			if (textureFrame > 3) textureFrame -= 3;

			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					window.close();
			}

			p.choiseMap();
			if (Keyboard::isKeyPressed(Keyboard::Left)) p.dx = -0.1;
			if (Keyboard::isKeyPressed(Keyboard::Right)) p.dx = 0.1;
			if (Keyboard::isKeyPressed(Keyboard::Up)) { if (p.onGround) { p.dy = -0.265; p.onGround = false; } }

			p.update(time);

			if (p.rect.left > weight / 2) offsetx = p.rect.left - weight / 2;
			if (p.rect.top > height / 2) offsety = p.rect.top - height / 2;

			window.clear(Color::Blue);

			window.draw(p.sprite);

			for (int y = 0; y < H; y++)
				for (int x = 0; x < W; x++) {
					if (Map[y][x] == 'S') stone.set(B, x, y, 10, 0);
					if (Map[y][x] == 'G') ground.set(B, x, y, 2, 0);
					if (Map[y][x] == 'L') lava.set(B, textureFrame, x, y, 6, 4, 5, 4, 4, 4, 1, 2, 0);
					if (Map[y][x] == 'A') {
						if (IsActive)ColAkt.setFillColor(Color::Green);
						else ColAkt.setFillColor(Color::Red);
						ColAkt.setPosition(x * 32 - offsetx, y * 32 - offsety);
					}
					if (Map[y][x] == 'I') InvStone.set(B, x, y, 11, 0);
					if ((Map[y][x] == '2') || (Map[y][x] == 'F')){
						ColFin.setFillColor(Color::Magenta);
						ColFin.setPosition(x * 32 - offsetx, y * 32 - offsety);
					}
					if (Map[y][x] == ' ') continue;

					if (visible) window.draw(InvStone.Block);
					window.draw(ColFin);
					window.draw(ColAkt);
					window.draw(stone.Block);
					window.draw(ground.Block);
					window.draw(lava.Block);
				}
			
			window.display();
		}

		return 0;
	}
