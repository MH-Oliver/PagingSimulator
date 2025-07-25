#ifndef PAGINGALGORITHM_H
#define PAGINGALGORITHM_H

/**
 * Die Abstrakte Basis-Klasse für alle Paging-Algorithmen
 */
class PagingAlgorithm {
public:
    virtual ~PagingAlgorithm() = default;

    /**
     * Informiert über einen Speicherzugriff auf einer spezifischen Seite.
     * Der Algorithmus kann dies verwenden, um seinen internen Zustand zu aktualisieren (z. B. ein referenziertes Bit zu setzen).
     * @param pageId Id der Seite, auf die zugegriffen wurde.
     */
    virtual void memoryAccess(int pageId) = 0;

    /**
     * Sobald der physische Speicher voll ist, wird hier ermittelt welche Seite zu räumen ist.
     * Hier findet die eigentliche Logik der jeweiligen Algorithmen statt.
     * @return Der Index des Rahmens im physischem Speicher, welcher entferrnt werden soll.
     */
    virtual int selectVictimPage() = 0;

    /**
     * Informiert den Algorithmus, dass eine neue Seite in einen bestimmten Frame geladen wurde.
     * Nützlich für Algorithmen, die Ladezeiten verfolgen müssen (wie FIFO).
     */
    virtual void pageLoaded(int pageId, int frameIndex) = 0;
};

#endif //PAGINGALGORITHM_H
