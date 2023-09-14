(require '[clojure.string :as str])
(def opcodes (map read-string (str/split (slurp "test_input.txt") #",")))

(println opcodes)