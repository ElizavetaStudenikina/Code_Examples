#include "Gamezone.h"
SUITS suit(int i){
	if(i < 8) return SPADES;
	if (i < 16) return CLUBS;
	if (i < 24) return DIAMONDS;
	if (i < 32) return HEARTS;
	return NONE;
};

int value(int i){
	return i%8;
};

const Gamezone Gamezone::operator=(const Gamezone& rp)
{
	for(int i = 0; i < 3; i++)
	{
		players_[i] = rp.players_[i];
		c_players_[i] = rp.c_players_[i];
		result_[i] = rp.result_[i];
		cards_[i] = rp.cards_[i];
		if(i !=2 ) handing_cards_[i] = rp.handing_cards_[i];
	}
	for(int i = 0; i < 32; i++)
	{
		options_[i] = rp.options_[i];
	}
	start_player_ = rp.start_player_;
	current_player_ = rp.current_player_;
	suit_ = rp.suit_;
	number_of_round_ = rp.number_of_round_;
	round_player_ = rp.round_player_;
	return *this;
};

 void Gamezone::round(){
	int card = 32;
	suit_ = NONE;
	//std::cout<<"ROUND "<<number_of_round_<<"\n";
	for(int j = 0; j < 4; j++)
	{
		cards_[j] = -1;
		if(j < 3) c_players_[j].clear_cards();
	}
	if(number_of_round_ < 3)
	{
		card = handing_cards_[number_of_round_ - 1];
		//std::cout<<"Player is table card is "<<card<<"\n";
		cards_[0] = card;
		for(int i = 0; i < 3; i++)
		{
			c_players_[i].drop_(card, 0);
			c_players_[i].table_card(card);
		}
		suit_ = suit(card);
		round_player_ = start_player_;
	}
	current_player_ = round_player_;
	for(int j = 0; j < 3; j++)
	{
		card = call_card(current_player_);
		if (j == 0 && number_of_round_ >= 3) suit_ = suit(card);
		for(int i = 0; i < 3; i++)
		{
			c_players_[i].drop_(card, current_player_ + 10);
			c_players_[i].table_card(card);
		}
		cards_[j + 1] = card;
		current_player_ = ++current_player_;
		current_player_ = norming(current_player_);
	}
	calculate_results();
	number_of_round_++;
};

Gamezone::Gamezone(gambler &pl_1, gambler &pl_2, gambler &pl_3){
	players_[0] = &pl_1;
	players_[1] = &pl_2;
	players_[2] = &pl_3;
	for(int i = 0; i < 3; i++) result_[i] = 0;
};


int Gamezone::call_card(int player_number){
	calculate_options(player_number);
	int j = players_[player_number]->call_card(c_players_[player_number], options_);
	if (j > 32)
	{
		int id = players_[player_number]->remember_configurate(c_players_[player_number], number_of_round_);
		players_[player_number]->turn(PLAY);
		int len = get_amount_of_options(options_);
		for(int i = 0; i < len; i++)
		{
			int card = get_card(options_, i);
			Gamezone ed = *this;
			int cur_res = result_[player_number];
			ed.arm_round(card);
			bool take = 0;
			if (result_[player_number] > cur_res) take = 1;
			ed.play_to_end();
			players_[player_number]->push_result(card, ed.get_result(player_number), take, id);
		}
		players_[player_number]->turn(DELAY);
		j = players_[player_number]->call_card(c_players_[player_number], options_);
		return j;
	}
	//std::cout<<"Player number "<<player_number<<" card is "<<j<<"\n";
	return j;
};

void Gamezone::arm_round(int card0){
	int card;
	int order = c_players_[current_player_].get_order();
	if(number_of_round_ < 3) order--;
	for(int j = order; j < 3; j++)
	{
		if(j != order) 
			card = call_card(current_player_);
		else 
		{
			card = card0;
			//std::cout<<"Player number "<<current_player_<<" card is "<<card<<"\n";
		}
		if (j == 0 && number_of_round_ >= 3) suit_ = suit(card);
		for(int i = 0; i < 3; i++)
		{
			c_players_[i].drop_(card, current_player_ + 10);
			c_players_[i].table_card(card);
		}
		cards_[j + 1] = card;
		current_player_ = addition_three(current_player_, 1);
	}
	calculate_results();
	number_of_round_++;
};

void Gamezone::calculate_options(int number){
	//c_players_[number].print();
	int  pointer = 0;
	for(int i = 0; i < 32; i++)
	{
		options_[i] = 0;
		if(c_players_[number].hand(i) &&(suit_ == NONE || suit_ == suit(i)))
		{
			options_[i] = 1;
			pointer = 1;
		}
	}
	if(pointer == 0)
	{
		for(int i = 0; i < 32; i++)
		{
			if(c_players_[number].hand(i))
				options_[i] = 1;
		}
	}
	return;
};

void Gamezone::calculate_results(){
	int loser = round_player_;
	int cur_player = round_player_;
	int lose_card_cost = value(cards_[1]);
	if(number_of_round_ < 3)
	{
		loser = 3;
		lose_card_cost = value(cards_[0]);
	}
	for(int i = 1; i < 4; i++)
	{
		if(suit_ == suit(cards_[i]) && value(cards_[i]) > lose_card_cost)
		{
			loser = cur_player;
			lose_card_cost = value(cards_[i]);
		}
		cur_player = addition_three(cur_player, 1);
	}
	//std::cout<<"Loser is player "<<loser<<"\n";
	if(loser < 3)
	{
		result_[loser]++;
		round_player_ = loser;
		return;
	}
	round_player_ = start_player_;
	return;
};

int Gamezone::play_game(){
	for(int i = 0; i < 3; i++) {
		result_[i] = 0;
		c_players_[i].restart(); 
	};
	deal();
	while(number_of_round_ <= 10) round();
	int loser = 0;
	for(int i = 1; i < 3; i ++)
	{
		if (result_[i] > result_[loser])
			loser = i;
	}
	return loser;
};

void Gamezone::play_to_end(){
	if(number_of_round_ == 0) play_game();
	else{
			while(number_of_round_ <= 10) round();
		}
	return;
};

void Gamezone::deal(){
	int cards[32];
	bool transposition = getRandomNumber(0, 1);
	int point, ran, k, boof;

	for(int i = 0; i < 32; i++) cards[i] = 3;
	for(int i = 0; i < 22; i++)
	{
		ran = getRandomNumber(1, 32 - i);
		k = 0;
		point = -1;
		while(k < ran)
		{
			if(cards[point + 1] == 3) k++;
			point++;
		}
		int a = 0;
		if(i >= 2 && i < 12) a = 1;
		if(i >= 12) a = 2;
		cards[point] = a;
	}	
	int number_hand_card = 0;
	for(int i = 0; i < 32; i++)
	{
		if(cards[i] == 0)
		{
			handing_cards_[number_hand_card] = i;
			number_hand_card++;
		}
		else 
		{
			c_players_[cards[i] - 1].push(i);
		}
	}
	if(transposition == 1)
	{
		boof = handing_cards_[0];
		handing_cards_[0] = handing_cards_[1];
		handing_cards_[1] = boof;
	}
	start_player_ = getRandomNumber(0, 2);
	number_of_round_ = 1;
	/*for(int i = 0; i < 32; i++)
		std::cout<<cards[i]<<" ";
	std::cout<<"\n";
	for(int j = 0; j < 3; j++)
	{
		std::cout<<"PLAYER "<<j<<"	";
		for(int i = 0; i < 32; i++)
		{
			if (c_players_[j].hand(i) ) std::cout<<i<<" ";
		}
		std::cout<<"\n";
	}*/
	return;
};




