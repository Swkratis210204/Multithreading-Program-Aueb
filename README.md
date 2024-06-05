# Multithreading-Program-Aueb
A multithreading Program, simulating a pizza shop. Developed for a semester exercise

Σύστημα παραγγελιών πίτσας με Multithreading
 Αυτή η αναφορά παρέχει μια επισκόπηση και ανάλυση ενός συστήματος
παραγγελιών πίτσας πολλαπλών νημάτων που εφαρμόζεται στο C. Το
σύστημα προσομοιώνει μια πιτσαρία που δέχεται τηλεφωνικές
παραγγελίες πελατών, προετοιμάζει πίτσες, τις ψήνει και τις παραδίδει
χρησιμοποιώντας πολλούς πόρους, όπως τηλέφωνα, παρασκευαστές,
φούρνους και διανομείς, κρατάει κάποια στοιχεία και στο τέλος της main
συνάρτησης τα εμφανίζει.
 Το σύστημα χρησιμοποιεί νήματα POSIX (pthreads) για να χειρίζεται πολλές
παραγγελίες πελατών ταυτόχρονα. Τα Mutex και οι μεταβλητές συνθήκης
(cond variables) χρησιμοποιούνται για τον συγχρονισμό της πρόσβασης σε
κοινόχρηστους πόρους, αποτρέποντας τα race conditions και συγκρούσεις
των πόρων. Οι κύριοι πόροι που διαχειρίζεται το σύστημα είναι τα τηλέφωνα
για την παραλαβή παραγγελιών, οι παρασκευαστές για την παρασκευή
πίτσας, οι φούρνοι για το ψήσιμο και οι διανομείς για την παράδοση της
πίτσας.
 Πριν φτάσουμε στο σημείο όπου τρέξουμε το πρόγραμμα χρειάζεται αν το
μεταγλωττίσουμε με την εντολή: gcc -pthread -o test test.c -lm. Το -lm το
προσθέτουμε γιατί χρησιμοποιούμε κάποιες συγκεκριμένες συναρτήσεις και
θέλουμε το σύστημα να τις καταλάβει. Ύστερα, τρέχουμε το πρόγραμμα,
δίνοντας δύο παραμέτρους, την πρώτη για τον αριθμό των
πελατών/τηλεφωνημάτων και μία για τον σπόρο που θα δώσουμε στην
συνάρτηση για τον υπολογισμό τυχαίων αριθμών.
 Όταν το πρόγραμμα ξεκινά ξεκινά, διαβάζει ένα header αρχείο με κάποιες
δοσμένες τιμές που τις δίνουμε σε μεταβλητές στο πρόγραμμα, παράδειγμα
τον αριθμό των φούρνων ή τον αριθμό των διανομέων. Κάθε παραγγελία
πελάτη υποβάλλεται σε επεξεργασία από ένα ξεχωριστό νήμα,
προσομοιώνοντας ολόκληρο τον κύκλο ζωής του νήματος από την
τοποθέτηση της παραγγελίας έως την παράδοση. Οι παραγγελίες γίνονται με
την απόκτηση μιας τηλεφωνικής γραμμής(ntel) και εάν δεν υπάρχουν
διαθέσιμες γραμμές, το νήμα περιμένει και μπλοκάρεται προσωρινά. Μετά
την εξασφάλιση μιας τηλεφωνικής γραμμής, δίνουμε σήμα στα νήματα να
ξυπνήσουν και να συνεχίσουν τη εκτέλεσή τους. Το πλήθος των πιτσών και τα
είδη τους καθορίζονται τυχαία, μέσω της συνάρτησης rand_r και είσοδο ένα
διάστημα που έχουμε πάρει από το header αρχείο . Στη συνέχεια, το σύστημα
προσομοιώνει τη χρέωση της κάρτας του πελάτη, με πιθανότητα αποτυχίας
της παραγγελίας εάν η χρέωση είναι ανεπιτυχής.
 Για τις επιτυχημένες παραγγελίες, το σύστημα αναθέτει τις πίτσες στους
παρασκευαστές. Εάν δεν υπάρχει διαθέσιμος παρασκευαστής, το νήμα
περιμένει, δηλαδή μπλοκάρεται προσωρινά, μέχρι να ελευθερωθεί ένας.
Μόλις ελευθερωθεί, το σύστημα ελέγχει για διαθέσιμους φούρνους και εάν
δεν υπάρχουν επαρκείς φούρνοι, το νήμα περιμένει. Μετά το ψήσιμο, η
παραγγελία συσκευάζεται και μεταφέρεται στην ουρά παράδοσης. Οι
παραγγελίες παραδίδονται από διαθέσιμους διανομείς, με το σύστημα να
περιμένει εάν κανένας διανομέας δεν είναι διαθέσιμος. Η διαδικασία
παράδοσης περιλαμβάνει τον χρόνο να φτάσει ο διανομέας στον πελάτη, να
παραδώσει την πίτσα και να γυρίσει πίσω.
 Στο τέλος της προσομοίωσης, το σύστημα εκτυπώνει διάφορα στατιστικά
στοιχεία: τα συνολικά έσοδα από επιτυχημένες παραγγελίες, τον συνολικό
αριθμό κάθε τύπου πίτσας που πωλήθηκε (Margherita, Pepperoni, Special),
τον αριθμό των επιτυχημένων και αποτυχημένων παραγγελιών, μέση και
μέγιστη εξυπηρέτηση φορές (από την τοποθέτηση της παραγγελίας έως την
παράδοση), και τους μέσους και μέγιστους χρόνους κρυώματος (από το
ψήσιμο έως την παράδοση). Αυτές οι μετρήσεις παρέχουν πληροφορίες για
την αποτελεσματικότητα των λειτουργιών του προγράμματός μας.
 Συνολικά, το σύστημα παραγγελίας πίτσας πολλαπλών νημάτων
προσομοιώνει αποτελεσματικά ένα πραγματικό περιβάλλον πιτσαρίας,
επιδεικνύοντας τη χρήση νημάτων για παράλληλους μηχανισμούς
επεξεργασίας και συγχρονισμού για τη διαχείριση των περιορισμών πόρων
με την χρήση των mutexes για το κλείδωμα των κρίσιμων περιοχών που
γίνονται οι βασικές λειτουργίες και την χρήση των condition variables για την
αφύπνιση των νημάτων όταν αυτό κριθεί κατάλληλο. Η ικανότητα του
συστήματος να παρακολουθεί μετρήσεις απόδοσης παρέχει πολύτιμες
πληροφορίες για τις λειτουργίες της πιτσαρίας.
Λεπτομερής Ανάλυση Συναρτήσεων και
Μεταβλητών
 Το σύστημα παραγγελίας πίτσας πολλαπλών νημάτων που εφαρμόζεται
στο C χρησιμοποιεί διάφορες συναρτήσεις και μεταβλητές για τη
διαχείριση της προσομοίωσης μιας πιτσαρίας. Αυτή η ενότητα παρέχει μια
λεπτομερή ανάλυση των βασικών συναρτήσεων και μεταβλητών που
χρησιμοποιούνται στον κώδικα.
Καθολικές Μεταβλητές
 Όρια πόρων και μετρητές:
o int ovens: Αριθμός διαθέσιμων φούρνων.
o int telephones: Αριθμός διαθέσιμων τηλεφωνικών γραμμών.
o int deliverers: Αριθμός διαθέσιμου διανομέων.
o int walterwhite: Αριθμός διαθέσιμων παρασκευαστών πίτσας
 Στατιστικοί μετρητές:
o int totalIncome: Συνολικά έσοδα από όλες τις επιτυχημένες
παραγγελίες.
o int totalPizzas[3]: Πίνακας που αποθηκεύει το πλήθος πωλήσεων
από κάθε τύπου πίτσας (Margherita, Pepperoni, Special).
o int totalOrders[2]: Πίνακας που αποθηκεύει το πλήθος των
επιτυχημένων και αποτυχημένων παραγγελιών.
o double totalServiceTime: Συνολκός χρόνος εκτέλεσης για όλα τα
προγράμματα (Βοηθητική μεταβλητή για να βρούμε τον μέσο χρόνο
εκτέλεσης).
o double maxServiceTime: Μέγιστος χρόνος εξυπηρέτησης για μία
παραγγελία.
o double totalCoolingTime: Συνολικός χρόνος κρυώματος για όλες
τις παραγγελίες προγράμματα (Βοηθητική μεταβλητή για να
βρούμε τον μέσο χρόνο κρυώματος).
o double maxCoolingTime: Μέγιστος χρόνος κρυώματος για μία
συγκεκριμένη παραγγελία.
 Συγχρονισμός:
o pthread_mutex_t lock1-7: Mutexes για προστασία κοινόχρηστων
πόρων.
o pthread_mutex_t print_lock: Mutex για προστασία πόρων κατά
την εκτύπωση
o pthread_cond_t cond_tel: Μεταβλητή συνθήκης για
διαθεσιμότητα τηλεφώνου.
o pthread_cond_t cond_prep: Μεταβλητή συνθήκης για
διαθεσιμότητα παρασκευαστή.
o pthread_cond_t cond_oven: Μεταβλητή συνθήκης για
διαθεσιμότητα φούρνου.
o pthread_cond_t cond_delivery: Μεταβλητή συνθήκης για
διαθεσιμότητα διανομέα.
 Structs:
o struct ThreadData: Struct που κρατάει δεδομένα για κάθε thread
πελάτη.
o int id: ID πελάτη.
o unsigned int seed: Σπόρος για τυχαίο αριθμό.
o int numPizzas: Αριθμός πιτσών της παραγγελίας.
o int chargeTime: Χρόνος για χρέωση της κάρτας.
o double chargeStatus: Αποθηκεύει αν πέτυχε η απέτυχε η
παραγγελία.
o time_t startTime: Χρονική στιγμή που άρχισε η παραγγελία.
o time_t endTime: Χρονική στιγμή που τελείωσε η παραγγελία.
o time_t startCoolingTime: Χρονική στιγμή που άρχισε το κρύωμα.
o time_t endCoolingTime: Χρονική στιγμή που τελείωσε το
κρύωμα.
 Συναρτήσεις:
o Main Function: Αρχικοποιεί το πρόγραμμα, επεξεργάζεται τις
παραμέτρους για τον αριθμό των πελατών και τον τυχαίο σπόρο και
δημιουργεί νήματα πελατών.
o Order Function: Επεξεργάζεται κάθε παραγγελία πελάτη.
Προσομοιώνει τον κύκλο ζωής μίας παραγγελίας, δηλαδή ενός
νήματος. Μέσα στην συνάρτηση αυτή, γίνονται όλες οι βασικές
λειτουργίες του προγράμματος που έχω περιγράψει και
παραπάνω, δηλαδή, το τηλεφώνημα για την καταχώρηση της
παραγγελίας, την προετοιμασία, το ψήσιμο, το πακετάρισμα και την
διανομή της.
 
