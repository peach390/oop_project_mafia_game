#include <iostream>
#include <string>
#include <vector>
#include<random>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include<algorithm>
#include <array>
#include "Example.h"

class Role {
 private:
    std :: string name;
    bool isKiller;
public:
    Role(const std:: string & name, const bool isKiller)
    : name(name), isKiller(isKiller) {

    }
    Role(const Role& other)
    : name(other.name),
      isKiller(other.isKiller) {
    }
    Role& operator= (const Role & other) {
        if (this != &other) {
            name = other.name;
            isKiller = other.isKiller;
        }
        return *this;
    }

    ~Role() {

    }

    const std :: string & getname() const {
        return name;
    }

    bool getisKiller() const {
        return isKiller;
    }

    friend std::ostream & operator<< (std::ostream & out, const Role & role) {
        out << "Role: " << role.getname()
        <<" | Killer: "
        <<(role.isKiller? "yes" : "no");
        return out;
    }


};

class Player {
private:

    std :: string nickname;
    Role role;
    bool isAlive;

public:

    Player(const std :: string & nickname, const Role & role, const bool isAlive)
    : nickname(nickname), role(role), isAlive(isAlive) {

    }

    Player(const Player& other)
    : nickname(other.nickname),
      role(other.role),
      isAlive(other.isAlive) {
    }

    Player& operator= (const Player & other) {
        if (this != &other) {
            nickname = other.nickname;
            role = other.role;
            isAlive = other.isAlive;
        }
        return *this;
    }


    ~Player() {

    }
    const std :: string & getnickname() const {
        return nickname;
    }
    const Role & getrole() const {
        return role;
    }
    void setRole(const Role & newRole) {
        role = newRole;
    }
    bool getisAlive() const {
        return isAlive;
    }
    void eliminate() {
        isAlive = false;
    }
    void revive() {
        isAlive = true;         // medicul aduce la viata un jucator
    }
    friend std::ostream& operator<<(std::ostream& out, const Player& player) {
        out << "Player: " << player.nickname
            << " | " << player.role
            << " | Alive: " << (player.isAlive ? "yes" : "no");
        return out;
    }
};


class Round {
private:
    int number;
    bool isDay;
    int noOfPlayers;
    std::vector<int> votes;

public:
    Round(int number, bool isDay, int noOfPlayers)
    : number(number),
      isDay(isDay),
      noOfPlayers(noOfPlayers),
      votes(noOfPlayers, 0) {
    }

    Round(const Round& other)
    : number(other.number),
      isDay(other.isDay),
      noOfPlayers(other.noOfPlayers),
      votes(other.votes) {
    }

    Round& operator= (const Round & other) {
        if (this != &other) {
            number = other.number;
            isDay = other.isDay;
            noOfPlayers = other.noOfPlayers;
            votes = other.votes;
        }
        return *this;
    }

    ~Round() {

    }
    int getnumber() const {
        return number;
    }
    bool getisDay() const {
        return isDay;
    }
    int getnoOfPlayers() const {
        return noOfPlayers;
    }
    const std::vector<int>& getvotes() const {
        return votes;
    }

    bool addvote(int i) {
        if (i >= 0 && i < noOfPlayers) {
            votes[i]++;
            return true;
        }
        return false;
    }

    void resetvote() {
        for (int i = 0; i < noOfPlayers; i++)
            votes[i]=0;
    }


    int getMostVotes() const {
        int maxVotes = 0;
        int imax = -1;
        bool tie = false;

        for (int i = 0; i < noOfPlayers; i++) {
            if (votes[i] > maxVotes) {
                maxVotes = votes[i];
                imax = i;
                tie = false;
            }
            else if (votes[i] == maxVotes && maxVotes > 0) {
                tie = true;
            }
        }

        if (tie) {
            return -1;
        }

        return imax;
    }

    void nextPhase() {
        if (isDay) {
            isDay = false;
        } else {
            isDay = true;
            number++;
        }
    }

    friend std::ostream & operator<< (std::ostream & out, const Round & round) {
        out << "Round: " << round.number
        <<" | Este zi: " << (round.isDay ? "yes" : "no")
        << " | Numar Jucatori: " << round.noOfPlayers<<std::endl;
        return out;
    }
};

class Game {
private:
    std::vector<Player> players;
    int gamesPlayed;
    Round roundaCurenta;
    int killerLastTarget;  //memoram pozita in vectorul jucatorilor, a jucatorului omorat in runda curenta
    int medicTarget;
    int medicLastSelfhealRound;
    int lastInvestigation;

public:
    Game(const std::vector<Player>& players,
     int gamesPlayed,
     const Round& roundaCurenta)
    : players(players),
      gamesPlayed(gamesPlayed),
      roundaCurenta(roundaCurenta),
      killerLastTarget(-1),
      medicTarget(-1),
      medicLastSelfhealRound(-1),
    lastInvestigation(-1) {
    }

    Game(const Game & other)
    : players(other.players),
    gamesPlayed(other.gamesPlayed),
    roundaCurenta(other.roundaCurenta),
    killerLastTarget(other.killerLastTarget),
    medicTarget(other.medicTarget),
    medicLastSelfhealRound(other.medicLastSelfhealRound),
    lastInvestigation(other.lastInvestigation) {

    }
    Game & operator= (const Game & other) {
        if (this != &other) {
            players = other.players;
            gamesPlayed = other.gamesPlayed;
            roundaCurenta = other.roundaCurenta;
            killerLastTarget = other.killerLastTarget;
            medicTarget = other.medicTarget;
            medicLastSelfhealRound = other.medicLastSelfhealRound;
            lastInvestigation = other.lastInvestigation;
        }
        return *this;
    }
    ~Game() {}

    void nextPhase() {
        roundaCurenta.nextPhase();
    }
    int getNumberOfPlayers() const {
        return static_cast<int>(players.size());
    }

    int getAlivePlayers() const {
        int noAlivePlayers = 0;
        for (int i = 0; i < getNumberOfPlayers(); i++) {
            if (players[i].getisAlive()) {
                noAlivePlayers++;
            }
        }
        return noAlivePlayers;
    }

    void showPlayers() const {
        for (int i = 0; i < getNumberOfPlayers(); i++) {
            std::cout << players[i];
        }
    }

    bool eliminatePlayers(int i) {
        if ( i <0 ||i >= getNumberOfPlayers()) {
            return false;
        }

        if (!players[i].getisAlive()) {
            return false;
        }

        players[i].eliminate();
        return true;
    }
    int checkWinner() const {

        int killer=0;
        int difrol=0;
        for (int i = 0; i < getNumberOfPlayers(); i++) {
            if (players[i].getisAlive()) {
                if (players[i].getrole().getisKiller())
                    killer++;
                else
                    difrol++;
            }
        }
        if (killer == 0 && difrol > 0)
            return 2;
        if (killer > 0 && difrol <= killer)
            return 1;
        return 0;

    }


    void assignRandomRoles() {
        if (getNumberOfPlayers() < 4) {
            return;
        }

        std::vector<Role> availableRoles;

        int numberOfKillers = getNumberOfPlayers() / 4;

        for (int i = 0; i < numberOfKillers; i++) {
            availableRoles.push_back(Role("Killer", true));
        }

        availableRoles.push_back(Role("Medic", false));
        availableRoles.push_back(Role("Politist", false));

        int numberOfCivilians = getNumberOfPlayers() - numberOfKillers - 2;

        for (int i = 0; i < numberOfCivilians; i++) {
            availableRoles.push_back(Role("Civil", false));
        }
        for (int i = static_cast<int>(availableRoles.size()) - 1; i > 0; i--) {
            int j = std::rand() % (i + 1);

            std::swap(availableRoles[i], availableRoles[j]);
        }

        for (int i=0; i<getNumberOfPlayers(); i++) {
            players[i].setRole(availableRoles[i]);
        }
    }

        bool chooseKillerTarget (int i) {
        if (roundaCurenta.getisDay())
            return false;
        if (i<0 || i >= getNumberOfPlayers())
            return false;

        if (!players[i].getisAlive()) {
            return false;
        }

        if (players[i].getrole().getisKiller())
            return false;

        killerLastTarget = i;
        return true;
    }
    bool isMedicAlive() const {
        for (int i = 0; i < getNumberOfPlayers(); i++) {
            if (players[i].getrole().getname() == "Medic" &&
                players[i].getisAlive()) {
                return true;
                }
        }

        return false;
    }

    bool chooseMedicTarget(int i) {
        if (roundaCurenta.getisDay())
            return false;

        if (!isMedicAlive())
            return false;

        if (i < 0 || i >= getNumberOfPlayers())
            return false;

        if (!players[i].getisAlive())
            return false;

        if (players[i].getrole().getname() == "Medic") {
            int currentRound = roundaCurenta.getnumber();

            if (medicLastSelfhealRound != -1 &&
                currentRound - medicLastSelfhealRound < 2)
                return false;
        }

        medicTarget = i;
        return true;
    }



    void resolveNight() {
        if (roundaCurenta.getisDay())
            return;

        int currentRound = roundaCurenta.getnumber();

        if (medicTarget >= 0 &&
            medicTarget < getNumberOfPlayers() &&
            players[medicTarget].getrole().getname() == "Medic") {

            medicLastSelfhealRound = currentRound;
            }

        if (killerLastTarget >= 0 &&
            killerLastTarget < getNumberOfPlayers() &&
            killerLastTarget != medicTarget &&
            players[killerLastTarget].getisAlive()) {

            players[killerLastTarget].eliminate();
            }

        killerLastTarget = -1;
        medicTarget = -1;
    }



    bool isPoliceAlive() const {
        for (int i = 0; i < getNumberOfPlayers(); i++) {
            if (players[i].getrole().getname() == "Politist" &&
                players[i].getisAlive()) {
                return true;
                }
        }

        return false;
    }


    bool policeInvestigate(int i, std::string& discoveredRole) {
        if (roundaCurenta.getisDay())
            return false;
        if (!isPoliceAlive()) {
            return false;
        }

        if (i < 0 || i >= getNumberOfPlayers()) {
            return false;
        }

        if (!players[i].getisAlive()) {
            return false;
        }

        if (players[i].getrole().getname() == "Politist") {
            return false;
        }
        int currentRound = roundaCurenta.getnumber();

        if (lastInvestigation == currentRound) {
            return false;
        }
        discoveredRole = players[i].getrole().getname();

        lastInvestigation = currentRound;

        return true;
    }
    void showPublicPlayers() const {
        for (int i = 0; i < getNumberOfPlayers(); i++) {
            std::cout << "Player: " << players[i].getnickname()
                      << " | Alive: "
                      << (players[i].getisAlive() ? "yes" : "no")
                      << '\n';
        }
    }

        friend std::ostream& operator<<(std::ostream& out, const Game& game) {
            out << game.roundaCurenta
                << "Games played: " << game.gamesPlayed
                << " | Number of players: " << game.getNumberOfPlayers()
                << '\n';

            for (int i = 0; i < game.getNumberOfPlayers(); i++) {
                out << game.players[i] << '\n';
            }

            return out;
        }
};


void runTests() {
    std::cout << "\n===== START TESTS =====\n";

    // 1. TEST ROLE
    Role killer("Killer", true);
    Role medic("Medic", false);
    Role civil("Civil", false);
    Role politist("Politist", false);

    assert(killer.getisKiller());
    assert(!medic.getisKiller());
    assert(civil.getname() == "Civil");

    Role copyRole(killer);
    assert(copyRole.getname() == "Killer");

    copyRole = medic;
    assert(copyRole.getname() == "Medic");


    // 2. TEST PLAYER
    Player p1("Maria", killer, true);
    Player p2("Ana", medic, true);
    Player p3("Andrei", civil, true);
    Player p4("Joh", politist, true);

    assert(p1.getnickname() == "Maria");
    assert(p1.getisAlive());
    assert(p1.getrole().getisKiller());

    p1.eliminate();
    assert(!p1.getisAlive());

    p1.revive();
    assert(p1.getisAlive());

    Player copyPlayer(p1);
    assert(copyPlayer.getnickname() == "Maria");

    copyPlayer = p2;
    assert(copyPlayer.getnickname() == "Ana");

    copyPlayer.setRole(civil);
    assert(copyPlayer.getrole().getname() == "Civil");


    // 3. TEST ROUND
    Round r1(1, true, 4);

    assert(r1.getnumber() == 1);
    assert(r1.getisDay());
    assert(r1.getnoOfPlayers() == 4);
    assert(r1.getvotes().size() == 4);

    assert(r1.addvote(2));
    assert(r1.addvote(2));
    assert(r1.addvote(1));

    assert(r1.getMostVotes() == 2);

    r1.resetvote();
    assert(r1.getMostVotes() == -1);

    assert(!r1.addvote(100));

    r1.nextPhase();
    assert(!r1.getisDay());
    assert(r1.getnumber() == 1);

    r1.nextPhase();
    assert(r1.getisDay());
    assert(r1.getnumber() == 2);

    Round copyRound(r1);
    assert(copyRound.getnumber() == 2);

    Round assignedRound(1, false, 4);
    assignedRound = r1;
    assert(assignedRound.getisDay());


    // 4. TEST GAME
    std::vector<Player> listaPlayers = {p1, p2, p3, p4};

    Round gameRound(1, true, 4);
    Game game1(listaPlayers, 0, gameRound);

    assert(game1.getNumberOfPlayers() == 4);
    assert(game1.getAlivePlayers() == 4);

    Game game2(game1);
    assert(game2.getAlivePlayers() == 4);

    game2.eliminatePlayers(1);
    assert(game2.getAlivePlayers() == 3);

    assert(!game2.eliminatePlayers(1));
    assert(!game2.eliminatePlayers(100));
    assert(!game2.eliminatePlayers(-1));

    // Copierea nu trebuie sa modifice originalul
    assert(game1.getAlivePlayers() == 4);

    game2 = game1;
    assert(game2.getAlivePlayers() == 4);

    assert(game1.checkWinner() == 0);

    // Victoria civililor
    Game gameCivili(listaPlayers, 0, gameRound);
    assert(gameCivili.eliminatePlayers(0));
    assert(gameCivili.checkWinner() == 2);

    // Victoria Mafiei
    Game gameMafia(listaPlayers, 0, gameRound);
    assert(gameMafia.eliminatePlayers(1));
    assert(gameMafia.eliminatePlayers(2));
    assert(gameMafia.checkWinner() == 1);


    // 5. TEST NIGHT ACTIONS
    Round nightRound(1, false, 4);
    Game nightGame(listaPlayers, 0, nightRound);

    assert(nightGame.isMedicAlive());
    assert(nightGame.isPoliceAlive());

    assert(nightGame.chooseKillerTarget(2));
    assert(nightGame.chooseMedicTarget(2));

    nightGame.resolveNight();

    // Medicul a protejat victima
    assert(nightGame.getAlivePlayers() == 4);


    // 6. TEST POLICE
    Game policeGame(listaPlayers, 0, nightRound);

    std::string discoveredRole;

    bool success = policeGame.policeInvestigate(0, discoveredRole);

    assert(success);
    assert(discoveredRole == "Killer");

    // Nu poate investiga de doua ori in aceeasi runda
    assert(!policeGame.policeInvestigate(1, discoveredRole));


    std::cout << "ALL TESTS PASSED!\n";
}


int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    runTests();

    return 0;


}
    /*
    std::cout << "Hello, world!\n";
    Example e1;
    e1.add(3);
    e1.g();
    std::array<int, 100> v{};
    int nr;
    std::cout << "Introduceți nr: ";
    */
    /////////////////////////////////////////////////////////////////////////
    /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    /// dați exemple de date de intrare folosind fișierul tastatura.txt
    /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    /// pentru cât mai multe ramuri de execuție.
    /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    ///
    /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    /// pentru a simula date introduse de la tastatură.
    /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    ///
    /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    /// program care merg (și să le evitați pe cele care nu merg).
    ///
    /////////////////////////////////////////////////////////////////////////
    ///std::cin >> nr;
    /////////////////////////////////////////////////////////////////////////


    /*
    for(int i = 0; i < nr; ++i) {
       std::cout << "v[" << i << "] = ";
       std::cin >> v[i];
    }
    std::cout << "\n\n";
    std::cout << "Am citit de la tastatură " << nr << " elemente:\n";
    for(int i = 0; i < nr; ++i) {
        std::cout << "- " << v[i] << "\n";
    }
    */
    ///////////////////////////////////////////////////////////////////////////
    /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    /// alt fișier propriu cu ce alt nume doriți.
    /// Exemplu:
    /// std::ifstream fis("date.txt");
    /// for(int i = 0; i < nr2; ++i)
    ///     fis >> v2[i];
    ///
    ///////////////////////////////////////////////////////////////////////////
    ///
    ///
