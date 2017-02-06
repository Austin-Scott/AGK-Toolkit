#pragma once
#include <agk.h>
#include <string>
#include <vector>
#include <math.h>
using namespace std;


class Animation {
private:
	string name;
	int height;
	int width;
	int framecount;
	float fps;
	bool loop;
	vector<int> imageIDs;
public:
	Animation() {
		name = "";
		height = -1;
		width = -1;
		framecount = -1;
		fps = 10;
		loop = true;
	}
	Animation(string animationName) {
		setName(animationName);
		height = -1;
		width = -1;
		framecount = -1;
		fps = 10;
		loop = true;
	}
	void setName(string animationName) {
		name = animationName;
	}
	string getName() {
		return name;
	}
	void setLoop(bool value) {
		loop = value;
	}
	bool getLoop() {
		return loop;
	}
	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	int getFrameCount() {
		return framecount;
	}
	float getFPS() {
		return fps;
	}
	vector<int> getImageIDs() {
		return imageIDs;
	}
	void destroy() {
		for (int i = 0; i < imageIDs.size(); i++) {
			imageIDs.erase(imageIDs.begin() + i);
			i--;
		}
	}
	void addFrame(string filename) {
		imageIDs.push_back(agk::LoadImage(filename.c_str()));
	}
	void setupSingleFrameAnimation(string filename, int frameWidth, int frameHeight, float newFPS = 10) {
		addFrame(filename);
		height = frameHeight;
		width = frameWidth;
		setFPS(newFPS);
		framecount = (agk::GetImageWidth(imageIDs[0]) / width)*(agk::GetImageHeight(imageIDs[0]) / height);
	}
	void setFPS(float newFPS) {
		fps = newFPS;
	}
};

class Entity {
private:
	int spriteID;
	vector<Animation> animations;
public:
	Entity() {
		spriteID = -1;
	}
	Entity(string imageFilename) {
		spriteID = -1;
		loadImage(imageFilename);
	}
	Entity(string imageFilename, float x, float y) {
		spriteID = -1;
		loadImage(imageFilename);
		setPos(x, y);
	}
	~Entity() {

	}
	void setPhysics(bool value, int mode=1/*1=static, 2=dynamic, 3=kinematic*/) {
		if (value) {
			agk::SetSpritePhysicsOn(spriteID, mode);
		}
		else {
			agk::SetSpritePhysicsOff(spriteID);
		}
	}
	void setSpriteShapeCircle() {
		agk::SetSpriteShape(spriteID, 1);
	}
	void setSpriteShapeSquare() {
		agk::SetSpriteShape(spriteID, 2);
	}
	void setSpriteShapePolygon() {
		agk::SetSpriteShape(spriteID, 3);
	}
	void setSpriteShapeSquare(int x, int y, int x2, int y2) {
		agk::SetSpriteShapeBox(spriteID, x, y, x2, y2);
	}
	void addAnimation(Animation anim) {
		animations.push_back(anim);
	}
	void setAnimation(string name) {
		for (int i = 0; i < animations.size(); i++) {
			if (name == animations[i].getName()) {
				agk::ClearSpriteAnimationFrames(spriteID);
				if (animations[i].getImageIDs().size() == 1) {
					agk::SetSpriteImage(spriteID, animations[i].getImageIDs()[0]);
					agk::SetSpriteAnimation(spriteID, animations[i].getWidth(), animations[i].getHeight(), animations[i].getFrameCount());
				}
				else {
					for (int x = 0; x < animations[i].getImageIDs().size(); x++) {
						agk::AddSpriteAnimationFrame(spriteID, animations[i].getImageIDs()[x]);
					}
				}
				agk::PlaySprite(spriteID, animations[i].getFPS(), animations[i].getLoop());
				break;
			}
		}
	}
	void stopAnimation() {
		agk::StopSprite(spriteID);
	}
	void resumeAnimation() {
		agk::ResumeSprite(spriteID);
	}
	bool isAnimationPlaying() {
		return (bool)agk::GetSpritePlaying(spriteID);
	}
	void destroy() {
		agk::DeleteSprite(spriteID);
	}
	void loadImage(string imageFilename) {
		if (spriteID != -1) {
			agk::SetSpriteImage(spriteID, agk::LoadImage(imageFilename.c_str()));
		}
		else {
			spriteID = agk::CreateSprite(agk::LoadImage(imageFilename.c_str()));
		}
	}
	void setImage(int imageID) {
		if (spriteID != -1) {
			agk::SetSpriteImage(spriteID, imageID);
		}
		else {
			spriteID = agk::CreateSprite(imageID);
		}
	}
	bool compare(Entity other) {
		if (spriteID == other.getID()) {
			return true;
		}
		else {
			return false;
		}
	}
	int getID() {
		return spriteID;
	}
	void setX(float x) {
		agk::SetSpriteX(spriteID, x);
	}
	void setY(float y) {
		agk::SetSpriteY(spriteID, y);
	}
	void setPos(float x, float y) {
		setX(x);
		setY(y);
	}
	float getX() {
		return agk::GetSpriteX(spriteID);
	}
	float getY() {
		return agk::GetSpriteY(spriteID);
	}
	void setSize(float w, float h) {
		agk::SetSpriteSize(spriteID, w, h);
	}
	float getW() {
		return agk::GetSpriteWidth(spriteID);
	}
	float getH() {
		return agk::GetSpriteHeight(spriteID);
	}
	bool isVisible() {
		return agk::GetSpriteVisible(spriteID);
	}
	void setVisible(bool value) {
		if (value) {
			agk::SetSpriteVisible(spriteID, 1);
		}
		else {
			agk::SetSpriteVisible(spriteID, 0);
		}
	}
	bool isActive() {
		return agk::GetSpriteActive(spriteID);
	}
	void setActive(bool value) {
		if (value) {
			agk::SetSpriteActive(spriteID, 1);
		}
		else {
			agk::SetSpriteActive(spriteID, 0);
		}
	}
	float getAngle() {
		return agk::GetSpriteAngle(spriteID);
	}
	void setAngle(float value) {
		agk::SetSpriteAngle(spriteID, value);
	}
	bool collision(Entity other) {
		return agk::GetSpriteCollision(spriteID, other.getID());
	}
	bool physicsCollision(Entity other) {
		return agk::GetPhysicsCollision(spriteID, other.getID());
	}

};

class Movement {
private:
	bool remove;
	int style; //0=move to point, 1=move towards point
	float speedx;
	float speedy;
	float speed;
	float endx;
	float endy;
	int tweentype;
	int tweenID;
public:
	Movement() {
		remove = true;
		style = 0; //0=move to point, 1=move towards point
		speedx = 0;
		speedy = 0;
		speed = 0;
		endx = 0;
		endy = 0;
		tweentype = 0;
		tweenID = -1;
	}
	~Movement() {

	}
	void set(bool shouldRemove, int objectStyle, float ispeedx, float ispeedy, float ispeed, float iendx, float iendy, int itweentype, int itweenID) {
		remove = shouldRemove;
		style = objectStyle;
		speedx = ispeedx;
		speedy = ispeedy;
		speed = ispeed;
		endx = iendx;
		endy = iendy;
		tweentype = itweentype;
		tweenID = itweenID;
	}
	bool getRemove() {
		return remove;
	}
	int getStyle() {
		return style;
	}
	float getSpeedX() {
		return speedx;
	}
	void setSpeedX(float value) {
		speedx = value;
	}
	void setSpeedY(float value) {
		speedy = value;
	}
	float getSpeedY() {
		return speedy;
	}
	float getSpeed() {
		return speed;
	}
	float getEndX() {
		return endx;
	}
	float getEndY() {
		return endy;
	}
	int getTweenType() {
		return tweentype;
	}
	int getTweenID() {
		return tweenID;
	}
	void setTweenID(int value) {
		tweenID = value;
	}

};

class DynamicEntity : public Entity {
private:
	vector<Movement> movementList;
	vector<Entity> collisionList;
	bool pause;
	int rampmove;
public:
	DynamicEntity() : Entity() {
		pause = false;
		rampmove = 0;
	}
	DynamicEntity(string imageFilename) : Entity(imageFilename) {
		pause = false;
		rampmove = 0;
	}
	DynamicEntity(string imageFilename, float x, float y) : Entity(imageFilename, x, y) {
		pause = false;
		rampmove = 0;
	}
	void addToEntityCollisionList(vector<Entity> newEntries) {
		for (int i = 0; i < newEntries.size(); i++) {
			collisionList.push_back(newEntries[i]);
		}
	}
	void eraseEntityCollisionList() {
		collisionList.erase(collisionList.begin(), collisionList.end());
	}
	void removeFromEntityCollisionList(vector<Entity> entriesToRemove) {
		for (int i = 0; i < collisionList.size(); i++) {
			for (int x = 0; x < entriesToRemove.size(); x++) {
				if (collisionList[i].compare(entriesToRemove[x])) {
					collisionList.erase(collisionList.begin() + i);
					i--;
					entriesToRemove.erase(entriesToRemove.begin() + x);
					break;
				}
			}
		}
	}
	void update(float delta) {
		if (movementList.size() > 0) {
			if (!pause) {
				if (movementList.at(0).getStyle() == 0) {
					if (movementList.at(0).getTweenID() == -1) {
						float deltax = movementList.at(0).getEndX() - getX();
						float deltay = movementList.at(0).getEndY() - getY();
						float distance = agk::Sqrt(agk::Pow(deltax, 2) + agk::Pow(deltay, 2));
						float time = distance / movementList.at(0).getSpeed();
						movementList.at(0).setTweenID(agk::CreateTweenCustom(time));
						agk::SetTweenCustomFloat1(movementList.at(0).getTweenID(), getX(), movementList.at(0).getEndX(), movementList.at(0).getTweenType());
						agk::SetTweenCustomFloat2(movementList.at(0).getTweenID(), getY(), movementList.at(0).getEndY(), movementList.at(0).getTweenType());
						agk::PlayTweenCustom(movementList.at(0).getTweenID(), 0);
					}
					else {
						if (agk::GetTweenCustomPlaying(movementList.at(0).getTweenID()) == 1) {
							agk::UpdateTweenCustom(movementList.at(0).getTweenID(), delta);
							setPos(agk::GetTweenCustomFloat1(movementList.at(0).getTweenID()), agk::GetTweenCustomFloat2(movementList.at(0).getTweenID()));
						}
						else {
							agk::DeleteTween(movementList.at(0).getTweenID());
							movementList.at(0).setTweenID(-1);
							if (!movementList.at(0).getRemove()) {
								Movement current = movementList[0];
								movementList.push_back(current);
							}
							movementList.erase(movementList.begin());
						}
					}
				}
				else if (movementList.at(0).getStyle() == 1) {
					if (movementList.at(0).getSpeedX() == -1) {
						float deltax = movementList.at(0).getEndX() - getX();
						float deltay = movementList.at(0).getEndY() - getY();
						float distance = agk::Sqrt(agk::Pow(deltax, 2) + agk::Pow(deltay, 2));
						float time = distance / movementList.at(0).getSpeed();
						movementList.at(0).setSpeedX(deltax / time);
						movementList.at(0).setSpeedY(deltay / time);
					}
					else {
						moveT(movementList.at(0).getSpeedX(), movementList.at(0).getSpeedY(), delta);
					}
				}
			}
		}
	}
	bool isMoving() {
		return movementList.size() > 0;
	}
	bool isPaused() {
		return pause;
	}
	void setPause(bool value) {
		pause = value;
	}

	void move(float dx, float dy) {
		setPos(getX() + dx, getY() + dy);
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - dx, getY() - dy);
					break;
				}
			}
		}
	}
	void moveT(float xPixelsPerSecond, float yPixelsPerSecond, float delta) {
		setPos(getX() + (xPixelsPerSecond*delta), getY() + (yPixelsPerSecond*delta));
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - (xPixelsPerSecond*delta), getY() - (yPixelsPerSecond*delta));
					break;
				}
			}
		}
	}
	void moveAtAngle(float distance, float angle) {
		setPos(getX() + (agk::Cos(angle)*distance), getY() + (agk::Sin(angle)*distance));
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - (agk::Cos(angle)*distance), getY() - (agk::Sin(angle)*distance));
					break;
				}
			}
		}
	}
	void moveAtAngleT(float distancePerSecond, float angle, float delta) {
		setPos(getX() + (agk::Cos(angle)*(distancePerSecond*delta)), getY() + (agk::Sin(angle)*(distancePerSecond*delta)));
		if (collisionList.size() > 0) {
			for (int i = 0; i < collisionList.size(); i++) {
				if (collision(collisionList[i])) {
					setPos(getX() - (agk::Cos(angle)*(distancePerSecond*delta)), getY() - (agk::Sin(angle)*(distancePerSecond*delta)));
					break;
				}
			}
		}
	}
	void rotate(float da) {
		setAngle(getAngle() + da);
	}
	void rotateT(float degreesPerSecond, float delta) {
		setAngle(getAngle() + (degreesPerSecond*delta));
	}
	void moveToPoint(float x, float y, float pixelsPerSecond, bool remove = true, int tween = 0) {
		movementList.push_back(Movement());
		movementList.at(movementList.size() - 1).set(remove, 0, -1, -1, pixelsPerSecond, x, y, tween, -1);
	}
	void moveTowardsPoint(float x, float y, float pixelsPerSecond) {
		movementList.push_back(Movement());
		movementList.at(movementList.size() - 1).set(false, 1, -1, -1, pixelsPerSecond, x, y, -1, -1);
	}
	void setRampMovePixels(int amount) {

	}
};

class timer {
private:
	float seconds;
	bool running;
	float startTime;
public:
	timer() {
		seconds = 0;
		startTime = 0;
		running = false;
	}
	timer(float time) {
		seconds = time;
		startTime = time;
		running = false;
	}
	void set(float time) {
		seconds = time;
		startTime = time;
	}
	void start() {
		running = true;
	}
	void stop() {
		running = false;
	}
	void reset() {
		seconds = startTime;
	}
	void update(float delta) {
		if (running) {
			seconds -= delta;
		}
	}
	float getTime() {
		return seconds;
	}
	bool check() {
		return !(seconds > 0);
	}

};

struct Tileset {
	int firstgid;
	int lastgid;
	string name;
	float tilewidth;
	float tileheight;
	int tilecount;
	int columns;
	int imageID;
};

struct Layer {
	string name;
	int width;
	int height;
	vector<Entity> sprites;
};

struct ImageLayer {
	int imageID;
	string name;
	float width;
	float height;
	float offsetx;
	float offsety;
};

struct Point {
	float x;
	float y;
};

struct MapObject {
	int id;
	string name;
	string otype;
	float x;
	float y;
	float width;
	float height;
	vector<Point> points;
};

class TmxMap {
private:
	vector<Layer> layers;
	vector<Tileset> tilesets;
	vector<ImageLayer> imageLayers;
	vector<MapObject> mapobjects;
	bool visible;
	bool active;
	float mapX;
	float mapY;
public:
	TmxMap() {
		visible = false;
		active = false;
		mapX = 0;
		mapY = 0;
	}
	TmxMap(string filename) {
		loadTmxMap(filename);
	}
	bool getVisible() {
		return visible;
	}
	bool getActive() {
		return active;
	}
	float getMapX() {
		return mapX;
	}
	float getMapY() {
		return mapY;
	}
	void setVisible(bool value) {
		for (int i = 0; i < layers.size(); i++) {
			for (int x = 0; x < layers[i].sprites.size(); x++) {
				layers[i].sprites[x].setVisible(value);
			}
		}
		for (int i = 0; i < imageLayers.size(); i++) {
			agk::SetSpriteVisible(imageLayers[i].imageID, (int)value);
		}
	}
	void setActive(bool value) {
		for (int i = 0; i < layers.size(); i++) {
			for (int x = 0; x < layers[i].sprites.size(); x++) {
				layers[i].sprites[x].setActive(value);
			}
		}
		for (int i = 0; i < imageLayers.size(); i++) {
			agk::SetSpriteActive(imageLayers[i].imageID, (int)value);
		}
	}
	void moveMap(float newX, float newY) {
		for (int i = 0; i < layers.size(); i++) {
			for (int x = 0; x < layers[i].sprites.size(); x++) {
				float offsetX = layers[i].sprites[x].getX() - mapX;
				float offsetY = layers[i].sprites[x].getY() - mapY;
				layers[i].sprites[x].setPos(newX + offsetX, newY + offsetY);
			}
		}
		for (int i = 0; i < imageLayers.size(); i++) {
			float offsetX = agk::GetSpriteX(imageLayers[i].imageID) - mapX;
			float offsetY = agk::GetSpriteY(imageLayers[i].imageID) - mapY;
			agk::SetSpriteX(imageLayers[i].imageID, newX + offsetX);
			agk::SetSpriteY(imageLayers[i].imageID, newY + offsetY);
		}
		for (int i = 0; i < mapobjects.size(); i++) {
			float offsetX = mapobjects[i].x - mapX;
			float offsetY = mapobjects[i].y - mapY;
			mapobjects[i].x = newX + offsetX;
			mapobjects[i].y = newY + offsetY;
			for (int z = 0; z < mapobjects[i].points.size(); z++) {
				float offX = mapobjects[i].points[z].x - mapX;
				float offY = mapobjects[i].points[z].y - mapY;
				mapobjects[i].points[z].x = newX + offX;
				mapobjects[i].points[z].y = newY + offY;
			}
		}

		mapX = newX;
		mapY = newY;
	}
	void loadTmxMap(string filename, float offsetX = 0, float offsetY = 0) {
		mapX = offsetX;
		mapY = offsetY;
		vector<string> data;
		int tmxDoc = agk::OpenToRead(filename.c_str());
		while (agk::FileEOF(tmxDoc) == 0) {
			data.push_back(agk::ReadLine(tmxDoc));
		}
		agk::CloseFile(tmxDoc);
		for (int x = 0; x < data.size(); x++) {
			if (agk::FindStringCount(data[x].c_str(), "tileset") > 0) {
				tilesets.emplace_back();
				tilesets[tilesets.size() - 1].firstgid = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 2));
				tilesets[tilesets.size() - 1].name = agk::GetStringToken(data[x].c_str(), "\"", 4);
				tilesets[tilesets.size() - 1].tilewidth = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 6));
				tilesets[tilesets.size() - 1].tileheight = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 8));
				tilesets[tilesets.size() - 1].tilecount = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 10));
				tilesets[tilesets.size() - 1].columns = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 12));
				x++;
				tilesets[tilesets.size() - 1].imageID = agk::LoadImage(agk::GetStringToken(data[x].c_str(), "\"", 2));
				tilesets[tilesets.size() - 1].lastgid = tilesets[tilesets.size() - 1].firstgid + (tilesets[tilesets.size() - 1].tilecount - 1);

				x++;
			}
			else if (agk::FindStringCount(data[x].c_str(), "imagelayer") > 0) {
				imageLayers.emplace_back();
				imageLayers[imageLayers.size() - 1].name = agk::GetStringToken(data[x].c_str(), "\"", 2);
				imageLayers[imageLayers.size() - 1].offsetx = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 4));
				imageLayers[imageLayers.size() - 1].offsety = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 6));
				x++;
				imageLayers[imageLayers.size() - 1].imageID = agk::CreateSprite(agk::LoadImage(agk::GetStringToken(data[x].c_str(), "\"", 2)));
				imageLayers[imageLayers.size() - 1].width = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 4));
				imageLayers[imageLayers.size() - 1].height = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 6));
				agk::SetSpriteSize(imageLayers[imageLayers.size() - 1].imageID, imageLayers[imageLayers.size() - 1].width, imageLayers[imageLayers.size() - 1].height);
				agk::SetSpritePosition(imageLayers[imageLayers.size() - 1].imageID, imageLayers[imageLayers.size() - 1].offsetx + offsetX, imageLayers[imageLayers.size() - 1].offsety + offsetY);

				x++;

			}
			else if (agk::FindStringCount(data[x].c_str(), "layer") > 0) {
				layers.emplace_back();
				layers[layers.size() - 1].name = agk::GetStringToken(data[x].c_str(), "\"", 2);
				layers[layers.size() - 1].width = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 4));
				layers[layers.size() - 1].height = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 6));
				x++;
				for (int h = 1; h <= layers[layers.size() - 1].height; h++) {
					x++;
					for (int w = 1; w <= layers[layers.size() - 1].width; w++) {
						int gid = agk::Val(agk::GetStringToken(data[x].c_str(), ",", w));
						if (gid != 0) {
							for (int i = 0; i < tilesets.size(); i++) {
								if (gid>tilesets[i].firstgid - 1 && gid < tilesets[i].lastgid + 1) {
									layers[layers.size() - 1].sprites.emplace_back();
									layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].setImage(tilesets[i].imageID);
									agk::SetSpriteAnimation(layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].getID(), tilesets[i].tilewidth, tilesets[i].tileheight, tilesets[i].tilecount);
									agk::SetSpriteFrame(layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].getID(), (gid - tilesets[i].firstgid) + 1);
									layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].setPos(((w - 1)*tilesets[i].tilewidth) + offsetX, ((h - 1)*tilesets[i].tileheight) + offsetY);
									agk::SetSpriteUVBorder(layers[layers.size() - 1].sprites[layers[layers.size() - 1].sprites.size() - 1].getID(), 1);

								}
							}
						}
					}
				}
				x += 2;
			}
			else if (agk::FindStringCount(data[x].c_str(), "objectgroup") > 0) {
				x++;
				while (!(agk::FindStringCount(data[x].c_str(), "/objectgroup") > 0)) {
					mapobjects.emplace_back();
					mapobjects[mapobjects.size() - 1].id = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 2));
					mapobjects[mapobjects.size() - 1].name = agk::GetStringToken(data[x].c_str(), "\"", 4);
					mapobjects[mapobjects.size() - 1].otype = agk::GetStringToken(data[x].c_str(), "\"", 6);
					mapobjects[mapobjects.size() - 1].x = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 8)) + offsetX;
					mapobjects[mapobjects.size() - 1].y = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 10)) + offsetY;
					mapobjects[mapobjects.size() - 1].width = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 12));
					mapobjects[mapobjects.size() - 1].height = agk::Val(agk::GetStringToken(data[x].c_str(), "\"", 14));
					x++;
					if (agk::FindStringCount(data[x].c_str(), "polyline") > 0) {
						for (int c = 1; c <= (agk::FindStringCount(data[x].c_str(), ",") * 2); c++) {
							Point p = Point();
							string cor = agk::GetStringToken(data[x].c_str(), "\"", 2);
							p.x = mapobjects[mapobjects.size() - 1].x + agk::Val(agk::GetStringToken(cor.c_str(), " ,", c));
							c++;
							p.y = mapobjects[mapobjects.size() - 1].y + agk::Val(agk::GetStringToken(cor.c_str(), " ,", c));
							mapobjects[mapobjects.size() - 1].points.push_back(p);
						}
						x++;
					}
				}
			}
		}
		visible = true;
		active = true;
	}
	vector<Entity> getTilesinLayer(string name) {
		for (int i = 0; i < layers.size(); i++) {
			if (name == layers[i].name) {
				return layers[i].sprites;
			}
		}
		return vector<Entity>();
	}
	vector<MapObject> getMapObjects(string name) {
		vector<MapObject> result;
		for (int i = 0; i < mapobjects.size(); i++) {
			if (name == mapobjects[i].name) {
				result.push_back(mapobjects[i]);
			}
		}
		return result;
	}

};

class Engine {
private:
	float offsetX;
	float offsetY;
public:
	Engine() {
		offsetX = 0;
		offsetY = 0;
	}
	Engine(string windowTitle, int width, int height, float fps = 60, bool fullscreen = false) {
		agk::SetVirtualResolution(width, height);
		agk::SetWindowSize(width, height, (int)fullscreen);
		agk::SetClearColor(0, 0, 0);
		agk::SetWindowTitle(windowTitle.c_str());
		agk::SetSyncRate(fps, 0);
		offsetX = 0;
		offsetY = 0;
	}
	void update() {
		agk::Sync();
	}
	float delta() {
		return agk::GetFrameTime();
	}
	float mouseX() {
		return agk::GetPointerX();
	}
	float mouseY() {
		return agk::GetPointerY();
	}
	float joyX() {
		return agk::GetJoystickX();
	}
	float joyY() {
		return agk::GetJoystickY();
	}
	bool click() {
		return (bool)agk::GetPointerPressed();
	}
	bool keyDown(int key) {
		return (bool)agk::GetRawKeyPressed(key);
	}
	void updateOffset() {
		offsetX = getViewOffsetX();
		offsetY = getViewOffsetY();
	}
	float getViewOffsetX() {
		return agk::GetViewOffsetX();
	}
	float getViewOffsetY() {
		return agk::GetViewOffsetY();
	}
	void setViewOffsetX(float value) {
		agk::SetViewOffset(value, offsetY);
		updateOffset();
	}
	void setViewOffsetRelX(float value) {
		agk::SetViewOffset(offsetX + value, offsetY);
		updateOffset();
	}
	void setViewOffsetY(float value) {
		agk::SetViewOffset(offsetX, value);
		updateOffset();
	}
	void setViewOffsetRelY(float value) {
		agk::SetViewOffset(offsetX, offsetY + value);
		updateOffset();
	}

};

class Sound {
private:
	int soundID;
	int instanceID;
public:
	Sound() {
		soundID = -1;
		instanceID = -1;
	}
	Sound(string filename) {
		soundID = -1;
		instanceID = -1;
		loadSound(filename);
	}
	void loadSound(string filename) {
		if (soundID == -1) {
			soundID = agk::LoadSound(filename.c_str());
		}
		else {
			agk::DeleteSound(soundID);
			soundID = agk::LoadSound(filename.c_str());
		}
	}
	int getID() {
		return soundID;
	}
	void play(int loop = 0) {
		instanceID = agk::PlaySound(soundID, 100, loop);
	}
	bool isPlaying() {
		return (bool)agk::GetSoundInstancePlaying(instanceID);
	}
	void stop() {
		agk::StopSoundInstance(instanceID);
	}
	void destroy() {
		agk::DeleteSound(soundID);
		soundID = -1;
	}
};

class Music {
private:
	int musicID;
public:
	Music() {
		musicID = -1;
	}
	Music(string filename) {
		musicID = -1;
		loadMusic(filename);
	}
	void loadMusic(string filename) {
		if (musicID == -1) {
			musicID = agk::LoadMusic(filename.c_str());
		}
		else {
			agk::DeleteMusic(musicID);
			musicID = agk::LoadMusic(filename.c_str());
		}
	}
	int getID() {
		return musicID;
	}
	void play(int loop = 0) {
		agk::PlayMusic(musicID, loop);
	}
	bool isPlaying() {
		return (bool)agk::GetMusicPlaying();
	}
	void stop() {
		agk::StopMusic();
	}
	void destroy() {
		agk::DeleteMusic(musicID);
		musicID = -1;
	}
};

class Font {
private:
	int fontID;
public:
	Font() {
		fontID = -1;
	}
	Font(string filename) {
		fontID = -1;
		loadFont(filename);
	}
	void loadFont(string filename) {
		fontID = agk::LoadFont(filename.c_str());
	}
	void destroy() {
		agk::DeleteFont(fontID);
		fontID = -1;
	}
	int getID() {
		return fontID;
	}

};

class Text {
private:
	Font ttFont;
	int textID;
public:
	Text() {
		ttFont = Font();
		textID = -1;
	}
	Text(Font ttf, string content, float size, float x, float y, int alignment) {

		textID = -1;
		createString(content);
		setFont(ttf);
		setSize(size);
		setX(x);
		setY(y);
		setAlignment(alignment);
	}
	void createString(string content) {
		if (textID != -1) {
			agk::DeleteText(textID);
		}
		textID = agk::CreateText(content.c_str());
	}
	void setFont(Font ttf) {
		ttFont = ttf;
		agk::SetTextFont(textID, ttFont.getID());
	}
	void setSize(float size) {
		agk::SetTextSize(textID, size);
	}
	void setX(float x) {
		agk::SetTextX(textID, x);
	}
	void setY(float y) {
		agk::SetTextY(textID, y);
	}
	string getString() {
		return agk::GetTextString(textID);
	}
	void setString(string content) {
		agk::SetTextString(textID, content.c_str());
	}
	void setColor(int r, int g, int b) {
		agk::SetTextColor(textID, r, g, b);
	}
	int getID() {
		return textID;
	}
	void fixTextToScreen() {
		agk::FixTextToScreen(textID, 1);
	}
	void setAlignment(int align) {
		agk::SetTextAlignment(textID, align);
	}
	void destroy() {
		agk::DeleteText(textID);
		textID = -1;
	}

};


