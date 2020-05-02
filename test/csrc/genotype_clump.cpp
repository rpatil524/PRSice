#include "catch.hpp"
#include "genotype.hpp"
#include "mock_genotype.hpp"

TEST_CASE("Build Clump window")
{
    mockGenotype geno;
    Reporter reporter("log", 60, true);
    geno.set_reporter(&reporter);
    std::vector<SNP> input = {SNP("rs1", 1, 10, "A", "C", 0, 10),
                              SNP("rs5", 1, 10, "A", "C", 0, 13),
                              SNP("rs3", 1, 10, "A", "C", 1, 10),
                              SNP("rs4", 1, 20, "A", "C", 1, 10),
                              SNP("rs10", 1, 25, "A", "C", 1, 10),
                              SNP("rs6", 1, 40, "A", "C", 1, 10),
                              SNP("rs7", 1, 70, "A", "C", 1, 10),
                              SNP("rs8", 3, 70, "A", "C", 1, 10)};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(input.begin(), input.end(), g);
    for (auto&& snp : input) { geno.load_snp(snp); }
    REQUIRE_NOTHROW(geno.build_clump_windows(10));
    auto res = geno.existed_snps();
    std::vector<std::string> expected_order = {"rs1",  "rs5", "rs3", "rs4",
                                               "rs10", "rs6", "rs7", "rs8"};
    REQUIRE(res.size() == expected_order.size());
    for (size_t i = 0; i < res.size(); ++i)
    {
        REQUIRE(res[i].rs() == expected_order[i]);
        switch (i)
        {
        case 0:
        case 1:
        case 2:
            REQUIRE(res[i].up_bound() == 4);
            REQUIRE(res[i].low_bound() == 0);
            break;
        case 3:
            REQUIRE(res[i].up_bound() == 5);
            REQUIRE(res[i].low_bound() == 0);
            break;
        case 4:
            REQUIRE(res[i].up_bound() == 5);
            REQUIRE(res[i].low_bound() == 3);
            break;
        case 5:
            REQUIRE(res[i].up_bound() == 6);
            REQUIRE(res[i].low_bound() == 5);
            break;
        case 6:
            REQUIRE(res[i].up_bound() == 7);
            REQUIRE(res[i].low_bound() == 6);
            break;
        case 7:
            REQUIRE(res[i].up_bound() == 8);
            REQUIRE(res[i].low_bound() == 7);
            break;
        }
    }
}
