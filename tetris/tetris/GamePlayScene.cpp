#include "Scene.h"
#include "GamePlayScene.h"
#include "stdafx.h"

GamePlayScene::GamePlayScene() {}
GamePlayScene::GamePlayScene(SceneNum num, GameClient* const pGameClient) {
	m_SceneNum = num;
	m_pGameClient = pGameClient;

}
GamePlayScene::~GamePlayScene() {}

void GamePlayScene::Update(float fTimeElapsed) {
    if (flag.game_reset == 0) {
        srand((unsigned)time(NULL)); //����ǥ���� 
        setcursortype(NOCURSOR); //Ŀ�� ����
        reset(); //������ ����
        flag.game_reset = 1;
    }
    /*__int64 LastTime = ::timeGetTime();
    __int64 CurrentTime;
    float fTimeScale = 0.001f;
    float fTimeElapsed = 0.0f;*/

    check_key(); //Ű�Է�Ȯ��
    // if (flag.crush_on && check_crush(bx, by + 1, b_rotation) == false);
    ////������ �浹���ΰ�� �߰��� �̵��� ȸ���� �ð��� ����
    //if (flag.space_key_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break;
    //    flag.space_key_on = 0;
    //    break;
    //}
    draw_main(); //ȭ���� �׸�
    /*while (fTimeElapsed < (1.0f / 60.0f)) {
        CurrentTime = ::timeGetTime();
        fTimeElapsed = (CurrentTime - LastTime) * fTimeScale;
    }
    LastTime = CurrentTime;*/
    m_gamestatus.fDropBlockTime += fTimeElapsed;
    drop_block(); // ������ ��ĭ ����
    check_level_up();  // �������� üũ
    check_game_over(); // ���ӿ����� üũ
    if (flag.new_block_on == 1) new_block(); // �� ���� flag�� �ִ� ��� ���ο� ���� ����
}

void GamePlayScene::reset(void) {
    m_gamestatus.level = 1; //�������� �ʱ�ȭ
    score = 0;
    level_goal = 1000;
    key = 0;
    flag.crush_on = 0;
    cnt = 0;
    m_gamestatus.speed = 1;

    system("cls"); //ȭ������
    reset_main(); // m_gamestatus.board_org�� �ʱ�ȭ
    draw_map(); // ����ȭ���� �׸�
    draw_main(); // �������� �׸�

    m_gamestatus.b_type_next = rand() % 7; //�������� ���� ���� ������ �����ϰ� ����
    new_block(); //���ο� ������ �ϳ� ����
}

void GamePlayScene::reset_main(void) { //�������� �ʱ�ȭ  
    int i, j;

    for (i = 0; i < BOARD_Y; i++) { // �������� 0���� �ʱ�ȭ  
        for (j = 0; j < BOARD_X; j++) {
            m_gamestatus.board_org[i][j] = 0;
            m_gamestatus.board_cpy[i][j] = 100;
        }
    }
    for (j = 1; j < BOARD_X; j++) { //y���� 3�� ��ġ�� õ���� ����
        m_gamestatus.board_org[3][j] = CEILLING;
    }
    for (i = 1; i < BOARD_Y - 1; i++) { //�¿� ���� ����  
        m_gamestatus.board_org[i][0] = WALL;
        m_gamestatus.board_org[i][BOARD_X - 1] = WALL;
    }
    for (j = 0; j < BOARD_X; j++) { //�ٴں��� ���� 
        m_gamestatus.board_org[BOARD_Y - 1][j] = WALL;
    }
}

void GamePlayScene::reset_main_cpy(void) { //m_gamestatus.board_cpy�� �ʱ�ȭ 
    int i, j;

    for (i = 0; i < BOARD_Y; i++) {         //�����ǿ� ���ӿ� ������ �ʴ� ���ڸ� ���� 
        for (j = 0; j < BOARD_X; j++) {  //�̴� m_gamestatus.board_org�� ���� ���ڰ� ���� �ϱ� ���� 
            m_gamestatus.board_cpy[i][j] = 100;
        }
    }
}

void GamePlayScene::draw_map(void) { //���� ���� ǥ�ø� ��Ÿ���� �Լ�  
    int y = 3;           // m_gamestatus.level, goal, score�� �����߿� ���� �ٲ�� �� ���� �� y���� ���� �����ص� 
                         // �׷��� Ȥ�� ���� ���� ǥ�� ��ġ�� �ٲ� �� �Լ����� �ȹٲ㵵 �ǰ�
    gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y); printf(" LEVEL : %5d", m_gamestatus.level);
    // gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y + 1); printf(" GOAL  : %5d", 10 - cnt);
    gotoxy(STATUS_X_ADJ, y + 2); printf("��    NEXT    ��");
    gotoxy(STATUS_X_ADJ, y + 3); printf("��            ��");
    gotoxy(STATUS_X_ADJ, y + 4); printf("��            ��");
    gotoxy(STATUS_X_ADJ, y + 5); printf("��            ��");
    gotoxy(STATUS_X_ADJ, y + 6); printf("��            ��");
    gotoxy(STATUS_X_ADJ, y + 7); printf("����������������������������");
    /*
    gotoxy(STATUS_X_ADJ, y + 8); printf(" YOUR SCORE :");
    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score);
    gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 11); printf("        %6d", last_score);
    gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);
    gotoxy(STATUS_X_ADJ, y + 15); printf("  ��   : Shift        SPACE : Hard Drop");
    gotoxy(STATUS_X_ADJ, y + 16); printf("��  �� : Left / Right   P   : Pause");
    gotoxy(STATUS_X_ADJ, y + 17); printf("  ��   : Soft Drop     ESC  : Quit");
    */
}

void GamePlayScene::draw_main(void) { //������ �׸��� �Լ� 
    int i, j;

    for (j = 1; j < BOARD_X - 1; j++) { //õ���� ��� ���ο������ �������� �������� ���� �׷��� 
        if (m_gamestatus.board_org[3][j] == EMPTY) m_gamestatus.board_org[3][j] = CEILLING;
    }
    for (i = 0; i < BOARD_Y; i++) {
        for (j = 0; j < BOARD_X; j++) {
            if (m_gamestatus.board_cpy[i][j] != m_gamestatus.board_org[i][j]) { //cpy�� ���ؼ� ���� �޶��� �κи� ���� �׷���.
                                                //�̰� ������ ��������ü�� ��� �׷��� �������� ��¦�Ÿ�
                gotoxy(BOARD_X_ADJ + j, BOARD_Y_ADJ + i);
                switch (m_gamestatus.board_org[i][j]) {
                case EMPTY: //��ĭ��� 
                    printf("  ");
                    break;
                case CEILLING: //õ���� 
                    printf(". ");
                    break;
                case WALL: //����� 
                    printf("��");
                    break;
                case INACTIVE_BLOCK: //���� ���� ���  
                    printf("��");
                    break;
                case ACTIVE_BLOCK: //�����̰��ִ� ���� ���
                    printf("��");
                    break;
                }
            }
        }
    }
    for (i = 0; i < BOARD_Y; i++) { //�������� �׸� �� m_gamestatus.board_cpy�� ����  
        for (j = 0; j < BOARD_X; j++) {
            m_gamestatus.board_cpy[i][j] = m_gamestatus.board_org[i][j];
        }
    }
}

void GamePlayScene::new_block(void) { //���ο� ���� ����  
    int i, j;

    m_gamestatus.bx = (BOARD_X / 2) - 1; //���� ���� ��ġx��ǥ(�������� ���) 
    m_gamestatus.by = 0;  //���� ������ġ y��ǥ(���� ��) 
    m_gamestatus.b_type = m_gamestatus.b_type_next; //������������ ������ 
    m_gamestatus.b_type_next = rand() % 7; //���� ������ ���� 
    m_gamestatus.b_rotation = 0;  //ȸ���� 0������ ������ 

    flag.new_block_on = 0; //new_block flag�� ��  

    for (i = 0; i < 4; i++) { //������ bx, by��ġ�� ��������  
        for (j = 0; j < 4; j++) {
            if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j] = ACTIVE_BLOCK;
        }
    }
    for (i = 1; i < 3; i++) { //���ӻ���ǥ�ÿ� ������ ���ú����� �׸� 
        for (j = 0; j < 4; j++) {
            if (blocks[m_gamestatus.b_type_next][0][i][j] == 1) {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("��");
            }
            else {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("  ");
            }
        }
    }
}

void GamePlayScene::check_key(void) {
    key = 0; //Ű�� �ʱ�ȭ  

    if (kbhit()) { //Ű�Է��� �ִ� ���  
        key = getch(); //Ű���� ����
        if (key == 224) { //����Ű�ΰ�� 
            do { key = getch(); } while (key == 224);//����Ű���ð��� ���� 
            switch (key) {
            case LEFT: //����Ű ��������
                if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                    if (check_crush(m_gamestatus.bx - 1, m_gamestatus.by, m_gamestatus.b_rotation) == true) move_block(LEFT);
                }
                break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
            case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
                if (check_crush(m_gamestatus.bx + 1, m_gamestatus.by, m_gamestatus.b_rotation) == true) move_block(RIGHT);
                break;
            case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
                if (check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == true) move_block(DOWN);
                break;
            case UP: //���� ����Ű ��������
                if (check_crush(m_gamestatus.bx, m_gamestatus.by, (m_gamestatus.b_rotation + 1) % 4) == true) move_block(UP);
                //ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
                else if (flag.crush_on == 1 && check_crush(m_gamestatus.bx, m_gamestatus.by - 1, (m_gamestatus.b_rotation + 1) % 4) == true) move_block(100);
            }   //�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
        }
        else { //����Ű�� �ƴѰ�� 
            switch (key) {
            case SPACE: //�����̽�Ű �������� 
                flag.space_key_on = 1; //�����̽�Ű flag�� ��� 
                while (flag.crush_on == 0) { //�ٴڿ� ���������� �̵���Ŵ
                    hard_drop_block();
                    score += m_gamestatus.level; // hard drop ���ʽ�
                }
                break;
            case ESC: //ESC�������� 
                system("cls"); //ȭ���� ����� 
                exit(0); //�������� 
            }
        }
    }
    while (kbhit()) getch(); //Ű���۸� ��� 
}

void GamePlayScene::drop_block() {
	if (m_gamestatus.fDropBlockTime >= m_gamestatus.speed) {
		if (flag.crush_on && check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == true) flag.crush_on = 0; //���� ��������� crush flag ��
		if (check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == true) move_block(DOWN); //���� ��������� ������ ��ĭ �̵�
		if (flag.crush_on && check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == false) { //���� ��������ʰ� crush flag�� ����������
			for (int i = 0; i < BOARD_Y; i++) { //���� �������� ������ ���� 
				for (int j = 0; j < BOARD_X; j++) {
					if (m_gamestatus.board_org[i][j] == ACTIVE_BLOCK) m_gamestatus.board_org[i][j] = INACTIVE_BLOCK;
				}
			}
			flag.crush_on = 0; //flag�� �� 
			check_line(); //����üũ�� �� 
			flag.new_block_on = 1; //���ο� �������� flag�� ��
			return; //�Լ� ���� 
		}
		if (check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == false) flag.crush_on = true; //������ �̵��� �ȵǸ�  crush flag�� ��
		m_gamestatus.fDropBlockTime = 0.0f;
	}
	return;
}

void GamePlayScene::hard_drop_block() {
	if (flag.crush_on && check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == true) flag.crush_on = 0; //���� ��������� crush flag ��
	if (check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == true) move_block(DOWN); //���� ��������� ������ ��ĭ �̵�
    if (flag.crush_on && check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == false) { //���� ��������ʰ� crush flag�� ����������
        for (int i = 0; i < BOARD_Y; i++) { //���� �������� ������ ����
            for (int j = 0; j < BOARD_X; j++) {
                if (m_gamestatus.board_org[i][j] == ACTIVE_BLOCK) m_gamestatus.board_org[i][j] = INACTIVE_BLOCK;
            }
        }
        flag.crush_on = 0; //flag�� �� 
        check_line(); //����üũ�� �� 
        flag.new_block_on = 1; //���ο� �������� flag�� ��
        return; //�Լ� ����
    }
	if (check_crush(m_gamestatus.bx, m_gamestatus.by + 1, m_gamestatus.b_rotation) == false) flag.crush_on = true; //������ �̵��� �ȵǸ�  crush flag�� ��
	m_gamestatus.fDropBlockTime = 0.0f;
    return;
}

int GamePlayScene::check_crush(int bx, int by, int b_rotation) { //������ ��ǥ�� ȸ�������� �浹�� �ִ��� �˻� 
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) { //������ ��ġ�� �����ǰ� ��������� ���ؼ� ��ġ�� false�� ���� 
            if (blocks[m_gamestatus.b_type][b_rotation][i][j] == 1 && m_gamestatus.board_org[by + i][bx + j] > 0) return false;
        }
    }
    return true; //�ϳ��� �Ȱ�ġ�� true���� 
};

void GamePlayScene::move_block(int dir) { //������ �̵���Ŵ 
    int i, j;

    switch (dir) {
    case LEFT: //���ʹ��� 
        for (i = 0; i < 4; i++) { //������ǥ�� ������ ���� 
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j] = EMPTY;
            }
        }
        for (i = 0; i < 4; i++) { //�������� ��ĭ���� active block�� ���� 
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j - 1] = ACTIVE_BLOCK;
            }
        }
        m_gamestatus.bx--; //��ǥ�� �̵� 
        break;

    case RIGHT:    //������ ����. ���ʹ����̶� ���� ������ ���� 
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j] = EMPTY;
            }
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j + 1] = ACTIVE_BLOCK;
            }
        }
        m_gamestatus.bx++;
        break;

    case DOWN:    //�Ʒ��� ����. ���ʹ����̶� ���� ������ ����
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j] = EMPTY;
            }
        }
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i + 1][m_gamestatus.bx + j] = ACTIVE_BLOCK;
            }
        }
        m_gamestatus.by++;
        break;

    case UP: //Ű���� ���� �������� ȸ����Ŵ. 
        for (i = 0; i < 4; i++) { //������ǥ�� ������ ����  
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j] = EMPTY;
            }
        }
        m_gamestatus.b_rotation = (m_gamestatus.b_rotation + 1) % 4; //ȸ������ 1������Ŵ(3���� 4�� �Ǵ� ���� 0���� �ǵ���) 
        for (i = 0; i < 4; i++) { //ȸ���� ������ ���� 
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j] = ACTIVE_BLOCK;
            }
        }
        break;

    case 100: //������ �ٴ�, Ȥ�� �ٸ� ���ϰ� ���� ���¿��� ��ĭ���� �÷� ȸ���� ������ ��� 
              //�̸� ���۽�Ű�� Ư������ 
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i][m_gamestatus.bx + j] = EMPTY;
            }
        }
        m_gamestatus.b_rotation = (m_gamestatus.b_rotation + 1) % 4;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (blocks[m_gamestatus.b_type][m_gamestatus.b_rotation][i][j] == 1) m_gamestatus.board_org[m_gamestatus.by + i - 1][m_gamestatus.bx + j] = ACTIVE_BLOCK;
            }
        }
        m_gamestatus.by--;
        break;
    }
}

void GamePlayScene::check_line(void) {
    int i, j, k, l;

    int    block_amount; //������ ���ϰ����� �����ϴ� ���� 
    int combo = 0; //�޺����� �����ϴ� ���� ������ �ʱ�ȭ 

    for (i = BOARD_Y - 2; i > 3;) { //i=MAIN_Y-2 : ���ʺ��� ��ĭ����,  i>3 : õ��(3)�Ʒ����� �˻� 
        block_amount = 0; //���ϰ��� ���� ���� �ʱ�ȭ 
        for (j = 1; j < BOARD_X - 1; j++) { //���� �������� ���ϰ��縦 �� 
            if (m_gamestatus.board_org[i][j] > 0) block_amount++;
        }
        if (block_amount == BOARD_X - 2) { //������ ���� �� ��� 
            if (flag.level_up_on == 0) { //���������°� �ƴ� ��쿡(�������� �Ǹ� �ڵ� �ٻ����� ����) 
                score += 100 * m_gamestatus.level; //�����߰� 
                cnt++; //���� �� ���� ī��Ʈ ���� 
                combo++; //�޺��� ����  
            }
            for (k = i; k > 1; k--) { //������ ��ĭ�� ��� ����(������ õ���� �ƴ� ��쿡��) 
                for (l = 1; l < BOARD_X - 1; l++) {
                    if (m_gamestatus.board_org[k - 1][l] != CEILLING) m_gamestatus.board_org[k][l] = m_gamestatus.board_org[k - 1][l];
                    if (m_gamestatus.board_org[k - 1][l] == CEILLING) m_gamestatus.board_org[k][l] = EMPTY;
                    //������ õ���� ��쿡�� õ���� ��ĭ ������ �ȵǴϱ� ��ĭ�� ���� 
                }
            }
        }
        else i--;
    }
    if (combo) { //�� ������ �ִ� ��� ������ ���� ��ǥ�� ���� ǥ����  
        if (combo > 1) { //2�޺��̻��� ��� ��� ���ʽ��� �޼����� �����ǿ� ����ٰ� ���� 
            gotoxy(BOARD_X_ADJ + (BOARD_X / 2) - 1, BOARD_Y_ADJ + m_gamestatus.by - 2); printf("%d COMBO!", combo);
            // Sleep(500);
            // score += (combo * m_gamestatus.level * 100);
            reset_main_cpy(); //�ؽ�Ʈ�� ����� ���� m_gamestatus.board_cpy�� �ʱ�ȭ.
        //(m_gamestatus.board_cpy�� m_gamestatus.board_org�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ������ ��ü�� ���� �׸��� ��) 
        }
        // gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
        // gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
    }
}

void GamePlayScene::check_level_up(void) {
    int i, j;

    if (cnt >= 10) { //�������� 10�پ� ���־���. 10���̻� ���� ��� 
        draw_main();
        flag.level_up_on = 1; //������ flag�� ���
        m_gamestatus.level += 1; //������ 1 �ø�
        cnt = 0; //���� �ټ� �ʱ�ȭ  

        reset_main_cpy(); //�ؽ�Ʈ�� ����� ���� m_gamestatus.board_cpy�� �ʱ�ȭ.
        //(m_gamestatus.board_cpy�� m_gamestatus.board_org�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ������ ��ü�� ���� �׸��� ��) 

        //.check_line()�Լ� ���ο��� m_gamestatus.level up flag�� �����ִ� ��� ������ ����.         
        switch (m_gamestatus.level) { //�������� �ӵ��� ��������. 
        case 2:
            m_gamestatus.speed = 0.9;
            break;
        case 3:
            m_gamestatus.speed = 0.8;
            break;
        case 4:
            m_gamestatus.speed = 0.7;
            break;
        case 5:
            m_gamestatus.speed = 0.6;
            break;
        case 6:
            m_gamestatus.speed = 0.5;
            break;
        case 7:
            m_gamestatus.speed = 0.4;
            break;
        case 8:
            m_gamestatus.speed = 0.3;
            break;
        case 9:
            m_gamestatus.speed = 0.2;
            break;
        case 10:
            m_gamestatus.speed = 0.1;
            break;
        }
        flag.level_up_on = 0; //������ flag����

        gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", m_gamestatus.level); //����ǥ�� 
        // gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // ������ǥ ǥ�� 

    }
}

void GamePlayScene::check_game_over(void) {
    int i;

    int x = 5;
    int y = 5;

    for (i = 1; i < BOARD_X - 2; i++) {
        if (m_gamestatus.board_org[3][i] > 0) { //õ��(������ ����° ��)�� inactive�� �����Ǹ� ���� ���� 
            gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�"); //���ӿ��� �޼��� 
            gotoxy(x, y + 1); printf("��                              ��");
            gotoxy(x, y + 2); printf("��  +-----------------------+   ��");
            gotoxy(x, y + 3); printf("��  |  G A M E  O V E R..   |   ��");
            gotoxy(x, y + 4); printf("��  +-----------------------+   ��");
            gotoxy(x, y + 5); printf("��   YOUR SCORE: %6d         ��", score);
            gotoxy(x, y + 6); printf("��                              ��");
            gotoxy(x, y + 7); printf("��  Press any key to restart..  ��");
            gotoxy(x, y + 8); printf("��                              ��");
            gotoxy(x, y + 9); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

            Sleep(1000);
            while (kbhit()) getch();
            key = getch();
            reset();
        }
    }
}