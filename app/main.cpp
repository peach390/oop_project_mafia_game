#include <iostream>
#include <string>
#include <vector>
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

};

int main() {
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
