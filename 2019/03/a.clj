(require '[clojure.string :as str])
(require '[closure.core.matrix :as matrix])
(def wires (str/split-lines (slurp "test_input.txt")))
(def grid (matrix/new-matrix 5000 5000))

;; (defn process-wire [wire] ())
