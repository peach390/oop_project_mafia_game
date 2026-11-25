#include <iostream>
#include <string>
#include <vector>
#include<random>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <limits>
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
            number++;
        } else {
            isDay = true;
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

    bool revealRolesOneByOne() const {
        std::string input;

        std::cout << "\n===== DISTRIBUIREA ROLURILOR =====\n";

        for (int i = 0; i < getNumberOfPlayers(); i++) {
            std::cout << "\nEste randul lui "
                      << players[i].getnickname() << ".\n";

            std::cout << "Doar acest jucator trebuie sa priveasca ecranul.\n";
            std::cout << "Apasa ENTER pentru a vedea rolul...";

            if (!std::getline(std::cin, input))
                return false;

            std::cout << "\nROLUL TAU: "
                      << players[i].getrole().getname() << '\n';

            std::cout << "\nMemoreaza rolul si apasa ENTER "
                         "pentru a-l ascunde...";

            if (!std::getline(std::cin, input))
                return false;

            std::cout << std::string(60, '\n');
        }

        std::cout << "\nToate rolurile au fost distribuite!\n";
        return true;
    }

    int countRole(const std::string& roleName) const {
        int count = 0;

        for (int i = 0; i < getNumberOfPlayers(); i++) {
            if (players[i].getrole().getname() == roleName) {
                count++;
            }
        }

        return count;
    }

    bool hideScreen() const {
        std::cout << "\nApasa ENTER pentru a ascunde informatiile...";

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );

        std::string line;
        if (!std::getline(std::cin, line))
            return false;

        std::cout << std::string(50, '\n');
        return true;
    }
    void showPublicPlayersWithNumbers() const {
        for (int i = 0; i < getNumberOfPlayers(); i++) {
            std::cout << i + 1 << ". "
                      << players[i].getnickname()
                      << " - "
                      << (players[i].getisAlive() ? "Viu" : "Eliminat")
                      << '\n';
        }
    }

void playGame() {
    while (checkWinner() == 0) {

        // =========================
        // ZIUA
        // =========================
        if (roundaCurenta.getisDay()) {
            std::cout << "\n===== ZIUA "
                      << roundaCurenta.getnumber()
                      << " =====\n";

            showPublicPlayersWithNumbers();
            roundaCurenta.resetvote();

            std::cout << "\nFiecare jucator viu voteaza.\n";
            std::cout << "Introdu 0 pentru abtinere.\n";

            for (int i = 0; i < getNumberOfPlayers(); i++) {
                if (!players[i].getisAlive())
                    continue;

                std::cout << "\nEste randul lui "
                          << players[i].getnickname()
                          << "\n";

                showPublicPlayersWithNumbers();

                int vote;

                while (true) {
                    std::cout << "Voteaza jucatorul (numar): ";

                    if (!(std::cin >> vote)) {
                        if (std::cin.eof())
                            return;

                        std::cin.clear();
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n'
                        );

                        std::cout << "Introdu un numar valid.\n";
                        continue;
                    }

                    if (vote == 0) {
                        std::cout << "Te-ai abtinut de la vot.\n";
                        break;
                    }

                    if (vote < 1 || vote > getNumberOfPlayers()) {
                        std::cout << "Vot invalid. Incearca din nou.\n";
                        continue;
                    }

                    int target = vote - 1;

                    if (!players[target].getisAlive()) {
                        std::cout << "Jucator deja eliminat. Voteaza din nou.\n";
                        continue;
                    }

                    roundaCurenta.addvote(target);
                    std::cout << "Votul a fost inregistrat.\n";
                    break;
                }

                if (!hideScreen())
                    return;
            }

            // REZULTATELE VOTULUI

            std::cout << "\n===== REZULTATELE VOTULUI =====\n";

            const std::vector<int>& votes = roundaCurenta.getvotes();

            for (int i = 0; i < getNumberOfPlayers(); i++) {
                if (players[i].getisAlive()) {
                    std::cout << players[i].getnickname()
                              << ": " << votes[i]
                              << " voturi\n";
                }
            }

            int mostVoted = roundaCurenta.getMostVotes();

            if (mostVoted != -1 &&
                players[mostVoted].getisAlive()) {

                eliminatePlayers(mostVoted);

                std::cout << "\nJucator eliminat prin vot: "
                          << players[mostVoted].getnickname()
                          << '\n';

            } else {
                std::cout << "\nEgalitate sau niciun vot valid."
                          << " Nimeni nu este eliminat.\n";
            }

            std::cout << "\nSituatia jucatorilor:\n";
            showPublicPlayersWithNumbers();

            roundaCurenta.resetvote();

            if (checkWinner() != 0)
                break;

            nextPhase();
        }

        // =========================
        // NOAPTEA
        // =========================
        if (!roundaCurenta.getisDay()) {
            std::cout << "\n===== NOAPTEA "
                      << roundaCurenta.getnumber()
                      << " =====\n";

            // KILLER
            int killerIndex = -1;

            for (int i = 0; i < getNumberOfPlayers(); i++) {
                if (players[i].getisAlive() &&
                    players[i].getrole().getisKiller()) {
                    killerIndex = i;
                    break;
                    }
            }

            if (killerIndex != -1) {
                std::cout << "\nEste randul Killer-ului.\n";
                showPublicPlayersWithNumbers();

                int target;
                bool success;

                do {
                    std::cout << "Alege victima: ";

                    if (!(std::cin >> target))
                        return;

                    success = chooseKillerTarget(target - 1);

                    if (!success)
                        std::cout << "Atac invalid. Alege din nou.\n";

                } while (!success);

                if (!hideScreen())
                    return;
            }

            // MEDIC
            if (isMedicAlive()) {
                std::cout << "\nEste randul Medicului.\n";
                showPublicPlayersWithNumbers();

                int target;
                bool success;

                do {
                    std::cout << "Alege persoana protejata: ";

                    if (!(std::cin >> target))
                        return;

                    success = chooseMedicTarget(target - 1);

                    if (!success)
                        std::cout << "Protectie invalida. Alege din nou.\n";

                } while (!success);

                if (!hideScreen())
                    return;
            }

            // POLITIST
            if (isPoliceAlive()) {
                std::cout << "\nEste randul Politistului.\n";
                showPublicPlayersWithNumbers();

                int target;
                bool success;
                std::string discoveredRole;

                do {
                    std::cout << "Alege persoana investigata: ";

                    if (!(std::cin >> target))
                        return;

                    success = policeInvestigate(target - 1, discoveredRole);

                    if (!success)
                        std::cout << "Investigatie invalida. Alege din nou.\n";

                } while (!success);

                std::cout << "\nRolul investigat este: "
                          << discoveredRole << '\n';

                if (!hideScreen())
                    return;
            }
            // REZOLVAREA NOPTII

            int attackedIndex = killerLastTarget;

            resolveNight();

            std::cout << "\n===== DIMINEATA =====\n";

            if (attackedIndex >= 0 &&
                attackedIndex < getNumberOfPlayers() &&
                !players[attackedIndex].getisAlive()) {

                std::cout << "\nJucatorul "
                          << players[attackedIndex].getnickname()
                          << " a fost eliminat in timpul noptii.\n";

            } else {
                std::cout << "\nNimeni nu a fost eliminat in aceasta noapte.\n";
            }

            std::cout << "\nSituatia jucatorilor:\n";
            showPublicPlayersWithNumbers();

            if (checkWinner() != 0)
                break;

            nextPhase();
        }
    }

    // =========================
    // FINALUL JOCULUI
    // =========================

    std::cout << "\n===== GAME OVER =====\n";

    int winner = checkWinner();

    if (winner == 1) {
        std::cout << "MAFIA A CASTIGAT!\n";
    } else if (winner == 2) {
        std::cout << "CIVILII AU CASTIGAT!\n";
    } else {
        std::cout << "Jocul s-a incheiat fara castigator.\n";
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

    // =========================
    // 1. TEST ROLE
    // =========================

    std::cout << "\nTesting Role...\n";

    Role killer("Killer", true);
    Role medic("Medic", false);
    Role civil("Civil", false);
    Role politist("Politist", false);

    assert(killer.getisKiller());
    assert(!medic.getisKiller());
    assert(civil.getname() == "Civil");

    Role copyRole(killer);
    assert(copyRole.getname() == "Killer");
    assert(copyRole.getisKiller());

    copyRole = medic;
    assert(copyRole.getname() == "Medic");
    assert(!copyRole.getisKiller());

    std::cout << "Role tests passed!\n";


    // =========================
    // 2. TEST PLAYER
    // =========================

    std::cout << "\nTesting Player...\n";

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

    // Modificarea copiei nu afecteaza originalul
    assert(p2.getrole().getname() == "Medic");

    std::cout << "Player tests passed!\n";


    // =========================
    // 3. TEST ROUND
    // =========================

    std::cout << "\nTesting Round...\n";

    Round r1(1, false, 4);

    assert(r1.getnumber() == 1);
    assert(!r1.getisDay());
    assert(r1.getnoOfPlayers() == 4);
    assert(r1.getvotes().size() == 4);

    // Noaptea 1 -> Ziua 1
    r1.nextPhase();

    assert(r1.getisDay());
    assert(r1.getnumber() == 1);

    // Testarea voturilor
    assert(r1.addvote(2));
    assert(r1.addvote(2));
    assert(r1.addvote(1));

    assert(r1.getMostVotes() == 2);

    // Resetarea voturilor
    r1.resetvote();

    assert(r1.getMostVotes() == -1);

    // Voturi invalide
    assert(!r1.addvote(100));
    assert(!r1.addvote(-1));

    // Egalitate de voturi
    assert(r1.addvote(1));
    assert(r1.addvote(2));

    assert(r1.getMostVotes() == -1);

    r1.resetvote();

    // Ziua 1 -> Noaptea 2
    r1.nextPhase();

    assert(!r1.getisDay());
    assert(r1.getnumber() == 2);

    // Noaptea 2 -> Ziua 2
    r1.nextPhase();

    assert(r1.getisDay());
    assert(r1.getnumber() == 2);

    // Constructor de copiere
    Round copyRound(r1);

    assert(copyRound.getnumber() == 2);
    assert(copyRound.getisDay());

    // Operator de atribuire
    Round assignedRound(1, false, 4);
    assignedRound = r1;

    assert(assignedRound.getisDay());
    assert(assignedRound.getnumber() == 2);

    std::cout << "Round tests passed!\n";


    // =========================
    // 4. TEST GAME
    // =========================

    std::cout << "\nTesting Game...\n";

    std::vector<Player> listaPlayers = {p1, p2, p3, p4};

    Round gameRound(1, true, 4);

    Game game1(listaPlayers, 0, gameRound);

    assert(game1.getNumberOfPlayers() == 4);
    assert(game1.getAlivePlayers() == 4);
    assert(game1.checkWinner() == 0);

    // Constructor de copiere
    Game game2(game1);

    assert(game2.getAlivePlayers() == 4);

    // Eliminarea unui jucator
    assert(game2.eliminatePlayers(1));
    assert(game2.getAlivePlayers() == 3);

    // Eliminari invalide
    assert(!game2.eliminatePlayers(1));
    assert(!game2.eliminatePlayers(100));
    assert(!game2.eliminatePlayers(-1));

    // Originalul ramane nemodificat
    assert(game1.getAlivePlayers() == 4);

    // Operator de atribuire
    game2 = game1;

    assert(game2.getAlivePlayers() == 4);

    // Victoria civililor
    Game gameCivili(listaPlayers, 0, gameRound);

    assert(gameCivili.eliminatePlayers(0));
    assert(gameCivili.checkWinner() == 2);

    // Victoria Mafiei
    Game gameMafia(listaPlayers, 0, gameRound);

    assert(gameMafia.eliminatePlayers(1));
    assert(gameMafia.eliminatePlayers(2));

    assert(gameMafia.checkWinner() == 1);

    std::cout << "Game tests passed!\n";


    // =========================
    // 5. TEST KILLER
    // =========================

    std::cout << "\nTesting Killer...\n";

    Round nightRound(1, false, 4);

    Game killerGame(listaPlayers, 0, nightRound);

    // Killer-ul nu poate ataca un jucator inexistent
    assert(!killerGame.chooseKillerTarget(100));
    assert(!killerGame.chooseKillerTarget(-1));

    // Killer-ul nu se poate ataca pe sine
    assert(!killerGame.chooseKillerTarget(0));

    // Eliminam un jucator
    assert(killerGame.eliminatePlayers(2));

    // Killer-ul nu poate ataca un jucator eliminat
    assert(!killerGame.chooseKillerTarget(2));

    // Killer-ul poate alege alta victima
    assert(killerGame.chooseKillerTarget(1));

    killerGame.resolveNight();

    assert(killerGame.getAlivePlayers() == 2);

    // Killer-ul nu poate ataca ziua
    Game dayGame(listaPlayers, 0, gameRound);

    assert(!dayGame.chooseKillerTarget(2));

    std::cout << "Killer tests passed!\n";


    // =========================
    // 6. TEST MEDIC
    // =========================

    std::cout << "\nTesting Medic...\n";

    Game medicGame(listaPlayers, 0, nightRound);

    assert(medicGame.isMedicAlive());

    // Protectii invalide
    assert(!medicGame.chooseMedicTarget(100));
    assert(!medicGame.chooseMedicTarget(-1));

    // Killer-ul ataca, Medicul protejeaza victima
    assert(medicGame.chooseKillerTarget(2));
    assert(medicGame.chooseMedicTarget(2));

    medicGame.resolveNight();

    assert(medicGame.getAlivePlayers() == 4);

    // Medicul protejeaza alta persoana
    Game deathGame(listaPlayers, 0, nightRound);

    assert(deathGame.chooseKillerTarget(2));
    assert(deathGame.chooseMedicTarget(1));

    deathGame.resolveNight();

    assert(deathGame.getAlivePlayers() == 3);

    // Medicul nu poate proteja un jucator eliminat
    Game invalidMedicGame(listaPlayers, 0, nightRound);

    assert(invalidMedicGame.eliminatePlayers(2));
    assert(!invalidMedicGame.chooseMedicTarget(2));

    // Poate alege alta persoana
    assert(invalidMedicGame.chooseMedicTarget(3));

    // Medicul eliminat nu mai poate proteja
    Game deadMedicGame(listaPlayers, 0, nightRound);

    assert(deadMedicGame.eliminatePlayers(1));
    assert(!deadMedicGame.isMedicAlive());
    assert(!deadMedicGame.chooseMedicTarget(2));

    // Medicul nu poate proteja ziua
    assert(!dayGame.chooseMedicTarget(2));

    std::cout << "Medic tests passed!\n";


    // =========================
    // 7. TEST MEDIC SELF-HEAL
    // =========================

    std::cout << "\nTesting Medic self-protection...\n";

    Game selfGame(listaPlayers, 0, nightRound);

    // Noaptea 1: se poate proteja
    assert(selfGame.chooseMedicTarget(1));

    selfGame.resolveNight();

    // Ziua 1
    selfGame.nextPhase();

    // Noaptea 2
    selfGame.nextPhase();

    // Nu se poate proteja doua nopti consecutive
    assert(!selfGame.chooseMedicTarget(1));

    // Poate proteja alta persoana
    assert(selfGame.chooseMedicTarget(2));

    selfGame.resolveNight();

    // Ziua 2
    selfGame.nextPhase();

    // Noaptea 3
    selfGame.nextPhase();

    // Se poate proteja din nou
    assert(selfGame.chooseMedicTarget(1));

    std::cout << "Medic self-protection tests passed!\n";


    // =========================
    // 8. TEST POLITIST
    // =========================

    std::cout << "\nTesting Politist...\n";

    Game policeGame(listaPlayers, 0, nightRound);

    std::string discoveredRole;

    assert(policeGame.isPoliceAlive());

    // Investigatii invalide
    assert(!policeGame.policeInvestigate(100, discoveredRole));
    assert(!policeGame.policeInvestigate(-1, discoveredRole));

    // Politistul nu se poate investiga pe sine
    assert(!policeGame.policeInvestigate(3, discoveredRole));

    // Investigatie valida
    assert(policeGame.policeInvestigate(0, discoveredRole));

    assert(discoveredRole == "Killer");

    // Nu poate investiga de doua ori in aceeasi noapte
    assert(!policeGame.policeInvestigate(1, discoveredRole));

    // Ziua 1
    policeGame.nextPhase();

    // Nu poate investiga ziua
    assert(!policeGame.policeInvestigate(2, discoveredRole));

    // Noaptea 2
    policeGame.nextPhase();

    // Poate investiga din nou
    assert(policeGame.policeInvestigate(2, discoveredRole));

    assert(discoveredRole == "Civil");

    // Nu poate investiga un jucator eliminat
    Game invalidPoliceGame(listaPlayers, 0, nightRound);

    assert(invalidPoliceGame.eliminatePlayers(2));

    assert(!invalidPoliceGame.policeInvestigate(
        2, discoveredRole
    ));

    // Poate alege alt jucator
    assert(invalidPoliceGame.policeInvestigate(
        0, discoveredRole
    ));

    // Politistul eliminat nu mai poate investiga
    Game deadPoliceGame(listaPlayers, 0, nightRound);

    assert(deadPoliceGame.eliminatePlayers(3));

    assert(!deadPoliceGame.isPoliceAlive());

    assert(!deadPoliceGame.policeInvestigate(
        0, discoveredRole
    ));

    std::cout << "Politist tests passed!\n";


    // =========================
    // 9. TEST RANDOM ROLES
    // =========================

    std::cout << "\nTesting random role assignment...\n";

    Role initialRole("Civil", false);

    for (int n : {4, 6, 8, 12}) {

        std::vector<Player> testPlayers;

        for (int i = 0; i < n; i++) {
            testPlayers.push_back(
                Player(
                    "Player" + std::to_string(i + 1),
                    initialRole,
                    true
                )
            );
        }

        Round testRound(1, false, n);

        Game randomGame(testPlayers, 0, testRound);

        randomGame.assignRandomRoles();

        assert(randomGame.getNumberOfPlayers() == n);
        assert(randomGame.getAlivePlayers() == n);

        assert(randomGame.countRole("Killer") == n / 4);
        assert(randomGame.countRole("Medic") == 1);
        assert(randomGame.countRole("Politist") == 1);

        int expectedCivilians = n - n / 4 - 2;

        assert(
            randomGame.countRole("Civil") == expectedCivilians
        );

        int totalRoles =
            randomGame.countRole("Killer") +
            randomGame.countRole("Medic") +
            randomGame.countRole("Politist") +
            randomGame.countRole("Civil");

        assert(totalRoles == n);

        std::cout << "Random roles test passed for "
                  << n << " players.\n";
    }


    // =========================
    // FINAL TESTS
    // =========================

    std::cout << "\n==========================\n";
    std::cout << "ALL TESTS PASSED!\n";
    std::cout << "==========================\n";
}


int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int option = -1;
    while (option != 0) {
        std::cout<< std::endl << "MAFIA GAME"<<std::endl;
        std::cout<< "1. Start joc"<<std::endl;
        std::cout<< "2. Reguli"<<std::endl;
        std::cout<< "3. ruleaza teste"<<std::endl;
        std::cout<< "0. Exit"<<std::endl;
        std::cout<< " Alege optiunea"<<std::endl;

        if (!(std::cin >> option)) {
            std::cout <<"Aceasta optiune nu exista."<<std::endl;
            break;
        }
        switch (option) {
            case 1:
                {
                std::cout<<"Configurare joc...";

                int numberOfPlayers;

                std::cout << "\nNumar de jucatori (minim 4): ";
                if (!(std::cin >> numberOfPlayers)) {
                    std::cout << "Date de intrare invalide.\n";
                    return 0;
                }

                if (numberOfPlayers < 4) {
                    std::cout << "Jocul necesita cel putin 4 jucatori.\n";
                    break;
                }

                std::vector<Player> gamePlayers;

                Role initialRole("Civil", false);

                for (int i = 0; i < numberOfPlayers; i++) {
                    std::string name;

                    std::cout << "Numele jucatorului " << i + 1 << ": ";
                    if (!(std::cin >> name)) {
                        std::cout << "Date de intrare insuficiente.\n";
                        return 0;
                    }

                    gamePlayers.push_back(Player(name, initialRole, true));
                }

                Round firstRound(1, false, numberOfPlayers);
                Game newGame(gamePlayers, 0, firstRound);

                newGame.assignRandomRoles();

                std::cout << "\nJocul a fost initializat!\n";
                newGame.showPublicPlayers();

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );

                if (newGame.revealRolesOneByOne()) {
                    newGame.playGame();
                }
                break;
            }


            case 2:
                std::cout << "\n===== REGULILE JOCULUI =====\n";
                std::cout << "Killer-ul elimina un jucator noaptea.\n";
                std::cout << "Medicul poate proteja un jucator noaptea.\n";
                std::cout << "Politistul poate investiga un jucator.\n";
                std::cout << "Ziua, jucatorii voteaza eliminarea unui suspect.\n";
                std::cout << "Rolurile raman secrete inclusiv dupa eliminare.\n";
                break;
            case 3:
                runTests();
                break;

            case 0:
                break;

            default:
                std::cout << "\nOptiune invalida!\n";
                break;
        }

    }

    //runTests();

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
