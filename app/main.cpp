#include <iostream>
#include <string>
#include <vector>
#include<random>
#include <cstdlib>
#include <ctime>
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

public:
    Game(const std::vector<Player>& players,
     int gamesPlayed,
     const Round& roundaCurenta)
    : players(players),
      gamesPlayed(gamesPlayed),
      roundaCurenta(roundaCurenta),
      killerLastTarget(-1),
      medicTarget(-1),
      medicLastSelfhealRound(-1) {
    }

    Game(const Game & other)
    : players(other.players),
    gamesPlayed(other.gamesPlayed),
    roundaCurenta(other.roundaCurenta),
    killerLastTarget(other.killerLastTarget),
    medicTarget(other.medicTarget),
    medicLastSelfhealRound(other.medicLastSelfhealRound) {

    }
    Game & operator= (const Game & other) {
        if (this != &other) {
            players = other.players;
            gamesPlayed = other.gamesPlayed;
            roundaCurenta = other.roundaCurenta;
            killerLastTarget = other.killerLastTarget;
            medicTarget = other.medicTarget;
            medicLastSelfhealRound = other.medicLastSelfhealRound;
        }
        return *this;
    }
    ~Game() {}

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
        bool chooseMedicTarget (int i) {

        if (!isMedicAlive()) {
            return false;
        }

        if (i<0 || i >= getNumberOfPlayers())
            return false;

        if (!players[i].getisAlive()) {
            return false;
        }

        if (players[i].getrole().getname() == "Medic" ) {

           int currentRound = roundaCurenta.getnumber();

            if (medicLastSelfhealRound != -1 && currentRound - medicLastSelfhealRound < 2)
                return false;
            medicLastSelfhealRound = currentRound;
        }
        medicTarget = i;
        return true;
    }

        void resoveNight (){
        if (killerLastTarget >= 0 && killerLastTarget < getNumberOfPlayers() && killerLastTarget != medicTarget) {
            players[killerLastTarget].eliminate();
            }

        killerLastTarget = -1;
        medicTarget = -1;
    }


    friend std::ostream& operator<<(std::ostream& out, const Game& game) {
        out << "Round: " << game.roundaCurenta.getnumber()
            << " | Games played: " << game.gamesPlayed
            << " | Number of players: " << game.getNumberOfPlayers()
            << '\n';

        for (int i = 0; i < game.getNumberOfPlayers(); i++) {
            out << game.players[i] << '\n';
        }

        return out;
    }

};



int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Role killer("Killer", true);
    Role medic("Medic", false);
    Role civil("Civil", false );
    Role politist ("Politist", false);
    // Test constructor
    std::cout << killer << '\n';

    // Test constructor de copiere
    Role copy = killer;
    std::cout << copy << '\n';

    // Test operator =
    copy = medic;
    std::cout << copy << '\n';

    copy = civil;
    std::cout << copy << '\n';

    copy = politist;
    std::cout << copy << '\n';

    Player p1("Maria", killer, true);
    std::cout << p1 << '\n';
    Player p2("Ana", medic, true);
    std::cout << p2 << '\n';
    Player p3("Andrei", civil, true);
    std::cout << p3 << '\n';
    Player p4("Joh", politist, true);
    std::cout << p4 << '\n';

    p1.eliminate();
    std:: cout << p1.getisAlive() << '\n';
    p1.revive();

    std::cout << p1.getisAlive() << '\n';

    Round r1(1, true, 4);

    r1.addvote(0);
    r1.addvote(2);
    r1.addvote(2);
    r1.addvote(3);
    r1.addvote(2);

    std::cout << r1;
    std::cout << "Most voted player: "
              << r1.getMostVotes() << '\n';

    r1.resetvote();

    std::cout << "After reset: "
              << r1.getMostVotes() << '\n';
    // Test constructor de copiere Player
    Player p5 = p1;
    std::cout << p5 << '\n';

    // Test operator= Player
    p5 = p2;
    std::cout << p5 << '\n';

    // Test constructor de copiere Round
    Round r2 = r1;
    std::cout << r2 << '\n';

    // Test operator= Round
    Round r3(2, false, 4);
    r3 = r1;
    std::cout << r3 << '\n';
    // Test getters Role
    std::cout << "Is killer: " << killer.getisKiller() << '\n';

    // Test getters Player
    std::cout << "Nickname: " << p1.getnickname() << '\n';
    std::cout << "Role: " << p1.getrole() << '\n';

    // Test getters Round
    std::cout << "Round number: " << r1.getnumber() << '\n';
    std::cout << "Is day: " << r1.getisDay() << '\n';
    std::cout << "Number of players: " << r1.getnoOfPlayers() << '\n';

    // Test votes
    std::cout << "Votes: ";
    for (int vote : r1.getvotes()) {
        std::cout << vote << " ";
    }
    std::cout << '\n';

    std::cout << "\n===== TEST GAME =====\n";

    // Crearea jocului
    std::vector<Player> listaPlayers = {p1, p2, p3, p4};
    Game game1(listaPlayers, 0, r1);

    // Testarea operatorului <<
    std::cout << "\nJocul initial:\n";
    std::cout << game1;

    // Testarea metodelor de afisare si numarare
    std::cout << "\nNumar total de jucatori: "
              << game1.getNumberOfPlayers() << '\n';

    std::cout << "Jucatori in viata: "
              << game1.getAlivePlayers() << '\n';

    std::cout << "\nLista jucatorilor:\n";
    game1.showPlayers();

    // Testarea constructorului de copiere
    Game game2(game1);
    std::cout << "\nJoc copiat:\n";
    std::cout << game2;

    // Testarea operatorului de atribuire
    Game game3(game1);
    game3 = game2;
    std::cout << "\nJoc dupa atribuire:\n";
    std::cout << game3;

    // Testarea eliminarii unui jucator
    std::cout << "\nEliminarea jucatorului cu index 1:\n";
    std::cout << "Eliminare reusita: "
              << game1.eliminatePlayers(1) << '\n';

    std::cout << "Jucatori ramasi in viata: "
              << game1.getAlivePlayers() << '\n';

    // Incercarea de a elimina din nou acelasi jucator
    std::cout << "Eliminare repetata: "
              << game1.eliminatePlayers(1) << '\n';

    // Testarea unui index invalid
    std::cout << "Eliminare index invalid: "
              << game1.eliminatePlayers(100) << '\n';

    // Verificarea conditiilor de victorie
    std::cout << "\nRezultat initial: "
              << game1.checkWinner() << '\n';

    // Eliminam inca un civil
    game1.eliminatePlayers(2);

    std::cout << "Rezultat dupa eliminarea a doi civili: "
              << game1.checkWinner() << '\n';

    // Testam separat victoria civililor
    Game gameCivili(listaPlayers, 0, r1);
    gameCivili.eliminatePlayers(0);

    std::cout << "Rezultat dupa eliminarea Killer-ului: "
              << gameCivili.checkWinner() << '\n';

    std::cout << "\n===== FINAL TEST GAME =====\n";

    std::cout << "\n===== TEST RANDOM ROLES =====\n";

    Game gameRandom(listaPlayers, 0, r1);

    std::cout << "\nBefore assigning random roles:\n";
    std::cout << gameRandom;

    gameRandom.assignRandomRoles();

    std::cout << "\nAfter assigning random roles:\n";
    std::cout << gameRandom;

    std::cout << "\n===== END RANDOM ROLES TEST =====\n";

    std::vector<Player> testPlayers = {
        Player("Maria", killer, true),
        Player("Ana", medic, true),
        Player("Andrei", civil, true),
        Player("Joh", politist, true)
    };

    Round testRound(1, false, 4);
    Game testGame(testPlayers, 0, testRound);

    std::cout << "Medic alive: "
              << testGame.isMedicAlive() << '\n';

    testGame.eliminatePlayers(1);

    std::cout << "Medic alive after elimination: "
              << testGame.isMedicAlive() << '\n';

    std::cout << "Medic can heal: "
              << testGame.chooseMedicTarget(2) << '\n';


    std::cout << "\n===== TEST NIGHT ACTIONS =====\n";

    // Cream un joc separat pentru testare
    std::vector<Player> nightPlayers = {
        Player("Maria", killer, true),
        Player("Ana", medic, true),
        Player("Andrei", civil, true),
        Player("Joh", politist, true)
    };

    Round nightRound(1, false, 4);
    Game nightGame(nightPlayers, 0, nightRound);

    // Killer-ul il alege pe Andrei (index 2)
    std::cout << "Killer target valid: "
              << nightGame.chooseKillerTarget(2) << '\n';

    // Medicul o protejeaza pe Maria (index 0)
    std::cout << "Medic target valid: "
              << nightGame.chooseMedicTarget(0) << '\n';

    // Procesam noaptea
    nightGame.resoveNight();

    std::cout << "Alive players after night: "
              << nightGame.getAlivePlayers() << '\n';

    std::cout << nightGame;

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
