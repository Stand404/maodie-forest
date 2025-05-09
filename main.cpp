#include <graphics.h>
#include <string>
#include <vector>
#include <iostream>
#include "ScoreManager.h"
#include "Atlas.h"
#include "Global.h"
#include "EnemyConfig.h"
#include "GameState.h"
#include <thread>
#include "GraphicsUtil.h"
#include "Buttons.h"
#include <windows.h>

GameState current_state = GameState::Loading;

bool running = true;
int level = 1;
int current_exp = 0;
int score = 0;
int last_score = 0;

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "Winmm.lib")

class Bullet {
public:
	POINT position = { 0, 0 };
	double rotation_angle = 0.0; // �ӵ�����ת�Ƕȣ����ȣ�

	Bullet() = default;

	~Bullet() = default;

	void Draw() const {
		// ������ʱ��תͼ��
		IMAGE img_rotated;

		// ������ת���ģ��ӵ�ͼƬ�����ģ�
		int center_x = img_bullet.getwidth() / 2;
		int center_y = img_bullet.getheight() / 2;

		// ��תͼ�񣨽Ƕ�ת��Ϊ�ȣ�
		rotateimage(
			&img_rotated,
			&img_bullet,
			rotation_angle / 1.3,  // ����ת�Ƕ�
			TRANSPARENT,  // ͸������
			true,        // ������С����Ӧ��ת
			false        // ���ü�
		);

		// ������ת���ͼ�񣨵���λ��ʹ�����Ķ��룩
		int draw_x = position.x - img_rotated.getwidth() / 2;
		int draw_y = position.y - img_rotated.getheight() / 2;

		putimage_alpha(draw_x, draw_y, &img_rotated);
	}
};

class Player
{
public:
	Player() {
		loadimage(&img_shadow, _T("resources/shadow.png"), SHADOW_WIDTH, SHADOW_WIDTH / 2);
		anim_left = new Animation(atlas_player_left, 45);
		anim_right = new Animation(atlas_player_right, 45);
	
		position.x = WINDOW_WIDTH / 2 - WIDTH / 2;
		position.y = WINDOW_HEIGHT / 2 - HEIGHT / 2 ;
	}

	~Player() {
		delete anim_left;
		delete anim_right;
	}

	void Reset() {
		is_move_up = false;
		is_move_down = false;
		is_move_left = false;
		is_move_right = false;

		position.x = WINDOW_WIDTH / 2 - WIDTH / 2;
		position.y = WINDOW_HEIGHT / 2 - HEIGHT / 2;
	}

	void ProcessEvent(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			switch (msg.vkcode)
			{
			case VK_UP:
			case 'W': // ���� W ��
				is_move_up = true;
				break;
			case VK_DOWN:
			case 'S': // ���� S ��
				is_move_down = true;
				break;
			case VK_LEFT:
			case 'A': // ���� A ��
				is_move_left = true;
				break;
			case VK_RIGHT:
			case 'D': // ���� D ��
				is_move_right = true;
				break;
			default:
				break;
			}
		}
		else if (msg.message == WM_KEYUP) {
			switch (msg.vkcode)
			{
			case VK_UP:
			case 'W': // �ͷ� W ��
				is_move_up = false;
				break;
			case VK_DOWN:
			case 'S': // �ͷ� S ��
				is_move_down = false;
				break;
			case VK_LEFT:
			case 'A': // �ͷ� A ��
				is_move_left = false;
				break;
			case VK_RIGHT:
			case 'D': // �ͷ� D ��
				is_move_right = false;
				break;
			default:
				break;
			}
		}
	}

	void Move() {
		// �ƶ�(������һ��)
		int dir_x = is_move_right - is_move_left;
		int dir_y = is_move_down - is_move_up;
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0) {
			double normalize_x = dir_x / len_dir;
			double normalize_y = dir_y / len_dir;
			position.x += (int)(SPEED * normalize_x);
			position.y += (int)(SPEED * normalize_y);
		}

		// ���ƽ�ɫ������
		if (position.x < MAP_AREA.x1) position.x = MAP_AREA.x1;
		if (position.y < MAP_AREA.y1) position.y = MAP_AREA.y1;
		if (position.x + WIDTH > MAP_AREA.x2) position.x = MAP_AREA.x2 - WIDTH;
		if (position.y + HEIGHT > MAP_AREA.y2) position.y = MAP_AREA.y2 - HEIGHT;
	}

	void Draw(int delta) {
		int pos_shadow_x = position.x + (WIDTH / 2 - SHADOW_WIDTH / 2);
		int pos_shadow_y = position.y + HEIGHT - 20;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

		static bool facing_left = false;
		int dir_x = is_move_right - is_move_left;
		if (dir_x < 0) facing_left = true;
		else if (dir_x > 0) facing_left = false;

		if (facing_left) anim_left->Play(position.x, position.y, delta);
		else anim_right->Play(position.x, position.y, delta);
	}

	const POINT& GetPosition() const {
		return position;
	}

public:
	const int SPEED = 5;
	const int WIDTH = 60;
	const int HEIGHT = 60;
	const int SHADOW_WIDTH = 50;

private:
	IMAGE img_shadow;
	Animation* anim_left;
	Animation* anim_right;
	POINT position = { 500, 500 };
	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;
};

class Enemy 
{
public:
	Enemy(EnemyType type) {
		const auto& config = enemy_configs[type];

		// ʹ�����ó�ʼ��
		loadimage(&img_shadow, config.shadow_path, config.shadow_width, config.shadow_width / 2);
		anim_left = new Animation(config.atlas_left, 45);
		anim_right = new Animation(config.atlas_right, 45);

		speed = config.speed;
		width = config.width;
		height = config.height;
		shadow_width = config.shadow_width;

		// ���ɱ߽�
		enum class SpawnEdge {
			Up = 0, Down, Left, Right
		};
		// ���λ��
		SpawnEdge edge = (SpawnEdge)(rand() % 4);
		switch (edge)
		{
		case SpawnEdge::Up:
			position.x = rand() % (MAP_AREA.x2 - MAP_AREA.x1) + MAP_AREA.x1;
			position.y = -height;
			break;
		case SpawnEdge::Down:
			position.x = rand() % (MAP_AREA.x2 - MAP_AREA.x1) + MAP_AREA.x1;
			position.y = WINDOW_HEIGHT;
			break;
		case SpawnEdge::Left:
			position.x = -width;
			position.y = rand() % (MAP_AREA.y2 - MAP_AREA.y1) + MAP_AREA.y1;
			break;
		case SpawnEdge::Right:
			position.x = WINDOW_WIDTH;
			position.y = rand() % (MAP_AREA.y2 - MAP_AREA.y1) + MAP_AREA.y1;
			break;
		default:
			break;
		}
	};
	~Enemy() {
		delete anim_left;
		delete anim_right;
	};

	bool CheckBulletCollision(const Bullet& bullet) {
		bool is_overlap_x = bullet.position.x > position.x && bullet.position.x < position.x + width;
		bool is_overlap_y = bullet.position.y > position.y && bullet.position.y < position.y + height;
		return is_overlap_x && is_overlap_y;
	}

	bool CheckPlayerCollision(const Player* player) {
		POINT check_point = { position.x + width / 2, position.y + height / 2 };
		POINT player_pos = player->GetPosition();
		bool is_overlap_x = check_point.x >= player_pos.x && check_point.x <= player_pos.x + width;
		bool is_overlap_y = check_point.y >= player_pos.y && check_point.y <= player_pos.y + height;
		return is_overlap_x && is_overlap_y;
	}

	void Move(const Player* player) {
		const POINT& player_position = player->GetPosition();
		int dir_x = player_position.x - position.x;
		int dir_y = player_position.y - position.y;

		if (dir_x < 0) facing_left = true;
		else if (dir_x > 0) facing_left = false;

		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0) {
			double normalize_x = dir_x / len_dir;
			double normalize_y = dir_y / len_dir;
			position.x += (int)(speed * normalize_x);
			position.y += (int)(speed * normalize_y);
		}
	}

	void Draw(int delta) {
		int pos_shadow_x = position.x + (width / 2 - shadow_width / 2);
		int pos_shadow_y = position.y + height / 1.3;
		putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

		if (facing_left) anim_left->Play(position.x, position.y, delta);
		else anim_right->Play(position.x, position.y, delta);
	}

	void Hurt() {
		alive = false;
	}

	bool CheckAlive() {
		return alive;
	}

private:
	EnemyType type;
	int speed;
	int width;
	int height;
	int shadow_width;
	IMAGE img_shadow;
	Animation* anim_left;
	Animation* anim_right;
	POINT position = { 0, 0 };
	bool facing_left = false;
	bool alive = true;
};

std::vector<Enemy*> enemy_list;
std::vector<Bullet> bullet_list(3);
Player* player;
Button* btn_start_game;
Button* btn_quit_game;
Button* btn_return_main;
Button* btn_open_github;
Button* btn_open_bili;

void TryGenerateEnemy (std::vector<Enemy*>& enemy_list) {
	const int INTERVAL = 100 / level;
	static int counter = 0;
	if ((++counter) % INTERVAL == 0) {
		// ���ѡ���������
		EnemyType types[] = {
			EnemyType::Normal,
			EnemyType::Type1,
			EnemyType::Type2,
			EnemyType::Type3,
			EnemyType::Type4,
			EnemyType::Type5,
			EnemyType::Type6
		};
		EnemyType random_type = types[rand() % (sizeof(types) / sizeof(types[0]))];

		enemy_list.push_back(new Enemy(random_type));
	}
}

void UpdateBullets(std::vector<Bullet>& bullet_list, Player* player) {
	const double RADIAL_SPEED = 0.0045;        // ���򲨶��ٶ�
	const double TANGENT_SPEED = 0.0055;      // ���򲨶��ٶ�
	const double ROTATION_SMOOTHNESS = 0.1;   // ��תƽ��ϵ����0.0~1.0��ԽСԽƽ����

	double radian_interval = 2 * 3.1415926 / bullet_list.size();
	POINT player_pos = player->GetPosition();
	double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);

	for (size_t i = 0; i < bullet_list.size(); i++) {
		double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;
		bullet_list[i].position.x = player_pos.x + player->WIDTH / 2 + (int)(radius * sin(radian));
		bullet_list[i].position.y = player_pos.y + player->HEIGHT / 2 + (int)(radius * cos(radian));

		// �����ӵ�����ҵ����λ��
		double dx = player_pos.x + player->WIDTH / 2 - bullet_list[i].position.x;
		double dy = player_pos.y + player->HEIGHT / 2 - bullet_list[i].position.y;

		// ����Ŀ��Ƕȣ�������ң�
		double target_angle = atan2(dy, dx);

		// ƽ�����ɵ�ǰ�Ƕȵ�Ŀ��Ƕ�
		double angle_diff = target_angle - bullet_list[i].rotation_angle;
		// ȷ���ǶȲ��� -�� �� �� ֮��
		if (angle_diff > 3.1415926) angle_diff -= 2 * 3.1415926;
		if (angle_diff < -3.1415926) angle_diff += 2 * 3.1415926;

		// Ӧ��ƽ����ת
		bullet_list[i].rotation_angle += angle_diff * ROTATION_SMOOTHNESS;
	}
}

void ResetGame(Player* player) {
	score = 0;
	current_exp = 0;

	for (size_t i = 0; i < enemy_list.size(); i++)
	{
		Enemy* enemy = enemy_list[i];
		std::swap(enemy_list[i], enemy_list.back());
		enemy_list.pop_back();
		delete enemy;
	}
	player->Reset();
}

void ReturnMainmenu() {
	// �����������ʷ��¼��
	if (score > last_score) {
		ScoreManager::WriteScore(score);
		last_score = score;
	}

	ResetGame(player);
	current_state = GameState::Waiting;
}

void RenderGame() {
	player->Move();
	TryGenerateEnemy(enemy_list);
	for (Enemy* enemy : enemy_list) enemy->Move(player);

	// ��ײ���
	for (Enemy* enemy : enemy_list) {
		if (enemy->CheckPlayerCollision(player)) {
			TCHAR szScore[10];
			_stprintf_s(szScore, _T("%d"), score);
			TCHAR szMessage[50];
			_stprintf_s(szMessage, _T("��ķ���Ϊ %s"), szScore);
			MessageBox(GetHWnd(), szMessage, _T("T^T"), MB_OK);

			ReturnMainmenu();
			break;
		}
	}
	for (Enemy* enemy : enemy_list) {
		for (const Bullet& bullet : bullet_list)
		{
			if (enemy->CheckBulletCollision(bullet)) {
				mciSendString(_T("play attack from 0"), NULL, 0, NULL);
				enemy->Hurt();
				score++;
				current_exp++;
				if (current_exp >= level * 10 && level < 100) {
					current_exp = 0;
					level++;
					// ����צ��������ֹ�޵�
					if (level < 8) bullet_list.push_back(Bullet());
				}
			}
		}
	}
	for (size_t i = 0; i < enemy_list.size(); i++)
	{
		Enemy* enemy = enemy_list[i];
		if (!enemy->CheckAlive()) {
			std::swap(enemy_list[i], enemy_list.back());
			enemy_list.pop_back();
			delete enemy;
		}
	}

	putimage(0, 0, &img_bg);
	player->Draw(1000 / 120);
	for (Enemy* enemy : enemy_list) enemy->Draw(1000 / 120);
	UpdateBullets(bullet_list, player);
	for (const Bullet& bullet : bullet_list) bullet.Draw();

	btn_return_main->Draw(); // ���ذ�ť
	// ����������� x ���꣬ʹ��λ�ڴ����м�
	int exp_bar_x = (WINDOW_WIDTH - EXP_BAR_WIDTH) / 2;

	// ���ƽ���������
	setfillcolor(WHITE);
	bar(exp_bar_x, EXP_BAR_Y, exp_bar_x + EXP_BAR_WIDTH, EXP_BAR_Y + EXP_BAR_HEIGHT);

	// ���ƽ�������䲿��
	int exp_bar_fill_width = (current_exp * EXP_BAR_WIDTH) / (level * 10); // �����������
	setfillcolor(GREEN);
	bar(exp_bar_x, EXP_BAR_Y, exp_bar_x + exp_bar_fill_width, EXP_BAR_Y + EXP_BAR_HEIGHT);

	// ���ƽ������߿�
	setlinecolor(BLACK);
	rectangle(exp_bar_x, EXP_BAR_Y, exp_bar_x + EXP_BAR_WIDTH, EXP_BAR_Y + EXP_BAR_HEIGHT);

	// �����ı�
	settextcolor(WHITE);
	settextstyle(30, 0, _T("΢���ź�"));
	std::wstring expText = L"����: " + std::to_wstring(current_exp) + L"/" + std::to_wstring(level * 10);
	outtextxy(exp_bar_x, EXP_BAR_Y - 40, expText.c_str()); // �ı�λ����΢����

	std::wstring levelText = L"�ȼ�: " + std::to_wstring(level) + L"/100";
	outtextxy(exp_bar_x + EXP_BAR_WIDTH - 120, EXP_BAR_Y - 40, levelText.c_str());

	std::wstring scoreText = L"����: " + std::to_wstring(score);
	outtextxy(10, 10, scoreText.c_str());
}

void RenderMainMenu() {
	main_anim->Play(0, 0, 1000 / 120);
	putimage_alpha(100, 100, &img_title);
	btn_start_game->Draw();
	btn_quit_game->Draw();
	btn_open_github->Draw();
	btn_open_bili->Draw();

	// ���Ʒ���
	settextcolor(WHITE);
	settextstyle(40, 0, _T("΢���ź�"));
	std::wstring scoreText = L"��ʷ���: " + std::to_wstring(last_score);
	outtextxy(110, 270, scoreText.c_str());
	std::wstring levelText = L"��ʱ�ȼ�: " + std::to_wstring(level);
	outtextxy(110, 310, levelText.c_str());
}

void RenderLoadingScreen() {
	putimage_alpha(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, &img_loading);

	// ��������
	settextcolor(WHITE);
	settextstyle(40, 0, _T("΢���ź�"));
	outtextxy(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 100, _T("������Դ��..."));
}

// ������Դ�������ں�̨�߳����У�
void LoadResources() {
	// 1. ����Logo/������Դ
	loadimage(&img_loading, _T("resources/loading.png"), 200, 100);

	// 2. Ԥ������Ϸ��Դ�����������Ϸ�󿨶٣�
	InitGameResources();
	InitEnemyConfigs();

	mciSendString(_T("open resources/audio/bgm.mp3 alias ready"), NULL, 0, NULL);

	// ��ʼ����ť
	btn_start_game = CreateStartButton(
		UIHelper::CalculateButtonRect(0),  // ��һ����ť������0��
		[]() {
			mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
			mciSendString(_T("open resources/audio/attack.mp3 alias attack"), NULL, 0, NULL);
			current_state = GameState::Running;
		}
	);

	btn_quit_game = CreateQuitButton(
		UIHelper::CalculateButtonRect(1),  // �ڶ�����ť������1��
		[]() {  running = false;  }
	);

	btn_return_main = new Button({ 1100, 20, 1260, 60 }, L"resources/return_btn/", []() { ReturnMainmenu(); });
	btn_open_github = new Button({ 1200, 640, 1260, 700 }, L"resources/github/", []() { ShellExecute(NULL, L"open", L"https://space.bilibili.com/382365750", NULL, NULL, SW_SHOWNORMAL);});
	btn_open_bili = new Button({ 1120, 640, 1180, 700 }, L"resources/bilibili/", []() { ShellExecute(NULL, L"open", L"https://github.com/stand114514/maodie-forest", NULL, NULL, SW_SHOWNORMAL);});

	last_score = ScoreManager::ReadScore(); // ��ȡ������¼

	player = new Player();
	// �л������˵�״̬
	mciSendString(_T("play ready repeat from 0"), NULL, 0, NULL);
	current_state = GameState::Waiting;
}

int main() {
#ifdef _DEBUG
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE); // �ڵ���ģʽ����ʾ����̨
#else
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT); // �ڷ���ģʽ�²���ʾ����̨
#endif

	// ������Դ�����߳�
	std::thread loading_thread(LoadResources);
	loading_thread.detach();  // �����̣߳���������ѭ��
	
	BeginBatchDraw();
	while (running)
	{
		DWORD start_time = GetTickCount();

		// ����
		ExMessage msg;
		while (peekmessage(&msg))
		{
			switch (current_state)
			{
			case GameState::Loading:
				break;
			case GameState::Waiting:
				btn_start_game->ProcessEvent(msg);
				btn_quit_game->ProcessEvent(msg);
				btn_open_github->ProcessEvent(msg);
				btn_open_bili->ProcessEvent(msg);
				break;
			case GameState::Running:
				player->ProcessEvent(msg);
				btn_return_main->ProcessEvent(msg);
				break;
			default:
				break;
			}
		}

		// ��Ⱦ
		cleardevice();
		switch (current_state)
		{
		case GameState::Loading:
			RenderLoadingScreen();
			break;
		case GameState::Waiting:
			RenderMainMenu();
			break;
		case GameState::Running:
			RenderGame();
			break;
		default:
			break;
		}
		FlushBatchDraw(); 

		// ֡������
		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		if (delta_time < 1000 / 120) {
			Sleep(1000 / 120 - delta_time);
		}
	}
	EndBatchDraw();

	CleanupGameResources();
	delete btn_start_game;
	delete btn_quit_game;
	delete btn_return_main;
	delete btn_open_github;
	delete btn_open_bili;
	delete player;

	return 0;
}