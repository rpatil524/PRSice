#ifndef MOCK_BINARYPLINK_HPP
#define MOCK_BINARYPLINK_HPP
#include "binaryplink.hpp"
#include "genotype.hpp"
#include <bitset>

class mock_binaryplink : public ::BinaryPlink
{
public:
    mock_binaryplink(GenoFile& geno, Phenotype& pheno, const std::string& delim,
                     Reporter* reporter)
        : BinaryPlink(geno, pheno, delim, reporter)
    {
    }
    mock_binaryplink() {}

    std::vector<uintptr_t> sample_for_ld() const { return m_sample_for_ld; }
    std::vector<uintptr_t> calculate_prs() const { return m_calculate_prs; }
    std::vector<Sample_ID> sample_id() const { return m_sample_id; }
    void test_check_bed(const std::string& bed_name, size_t num_marker,
                        uintptr_t& bed_offset)
    {
        check_bed(bed_name, num_marker, bed_offset);
    }
    bool test_calc_freq_gen_inter(const QCFiltering& filter_info)
    {
        return calc_freq_gen_inter(filter_info, "", this);
    }
    void manual_load_snp(SNP cur)
    {
        m_existed_snps_index[cur.rs()] = m_existed_snps.size();
        m_existed_snps.emplace_back(cur);
    }
    std::vector<std::pair<size_t, size_t>> test_get_chrom_boundary()
    {
        return get_chrom_boundary();
    }
    std::vector<size_t> sorted_p_index() { return m_sort_by_p_index; }
    std::vector<SNP>& existed_snps() { return m_existed_snps; }
    void set_sample(uintptr_t n_sample) { m_unfiltered_sample_ct = n_sample; }
    void set_reporter(Reporter* reporter) { m_reporter = reporter; }
    void test_post_sample_read_init() { post_sample_read_init(); }
    void test_init_sample_vectors() { init_sample_vectors(); }
    size_t test_transverse_bed_for_snp(
        const std::vector<IITree<size_t, size_t>>& exclusion_regions,
        const std::string mismatch_snp_record_name, const size_t idx,
        const uintptr_t unfiltered_sample_ct4, const uintptr_t bed_offset,
        std::unique_ptr<std::istream> bim,
        std::unordered_set<std::string>& duplicated_snps,
        std::unordered_set<std::string>& processed_snps,
        std::vector<bool>& retain_snp, bool& chr_error, bool& sex_error,
        Genotype* genotype)
    {
        return transverse_bed_for_snp(
            exclusion_regions, mismatch_snp_record_name, idx,
            unfiltered_sample_ct4, bed_offset, std::move(bim), duplicated_snps,
            processed_snps, retain_snp, chr_error, sex_error, genotype);
    }
    void test_read_genotype(const SNP& snp, uintptr_t* genotype, bool is_ref)
    {
        read_genotype(snp, m_founder_ct, m_genotype_file, m_tmp_genotype.data(),
                      genotype, m_sample_for_ld.data(), is_ref);
    }
    void test_read_genotype(uintptr_t* genotype, SNP& snp)
    {
        read_genotype(snp, m_founder_ct, m_genotype_file, m_tmp_genotype.data(),
                      genotype, m_sample_for_ld.data());
    }
    void gen_fake_bed_from_int(const std::vector<std::vector<uintptr_t>>& geno,
                               const std::string& name,
                               const size_t sample_size)
    {
        std::ofstream plink(name + ".bed", std::ios::binary);
        m_genotype_file_names.clear();
        m_genotype_file_names.push_back(name);
        std::bitset<8> b;
        char ch[1];
        // generate header so we can use plink to validate our file

        b.reset();
        b.set(2);
        b.set(3);
        b.set(5);
        b.set(6);
        ch[0] = static_cast<char>(b.to_ulong());
        plink.write(ch, 1);
        b.reset();
        b.set(0);
        b.set(1);
        b.set(3);
        b.set(4);
        ch[0] = static_cast<char>(b.to_ulong());
        plink.write(ch, 1);
        b.reset();
        b.set(0);
        ch[0] = static_cast<char>(b.to_ulong());
        plink.write(ch, 1);
        for (auto&& g : geno)
        {
            size_t c = 0;
            size_t i = 0;
            size_t idx = 0;
            while (i < g.size() && idx < sample_size * 2)
            {
                c = 0;
                b.reset();
                while (c < 8 && idx < sample_size * 2)
                {
                    if (IS_SET(g.data(), idx)) { b.set(c); }
                    ++c;
                    ++idx;
                    i = idx / BITCT;
                }
                ch[0] = static_cast<char>(b.to_ulong());
                plink.write(ch, 1);
            }
        }
        plink.close();
    }
    void test_init_chr(int num_auto = 22, bool no_x = false, bool no_y = false,
                       bool no_xy = false, bool no_mt = false)
    {
        init_chr(num_auto, no_x, no_y, no_xy, no_mt);
    }
    void add_file_name(const std::string& in)
    {
        m_genotype_file_names.push_back(in);
    }
    void set_founder_vector(const size_t n_sample)
    {
        for (size_t i = 0; i < n_sample; ++i)
        { SET_BIT(i, m_sample_for_ld.data()); }
        m_founder_ct = n_sample;
    }
    void gen_fake_bed(const std::vector<std::vector<size_t>>& geno,
                      const std::string& name)
    { // it is a real bed file, but without the header
        std::ofstream plink(name + ".bed", std::ios::binary);
        m_existed_snps.clear();
        m_existed_snps.push_back(SNP("rs", 1, 1, "A", "T", 0, 3));
        m_genotype_file_names.clear();
        m_genotype_file_names.push_back(name);
        std::bitset<8> b;
        char ch[1];
        // generate header so we can use plink to validate our file

        b.reset();
        b.set(2);
        b.set(3);
        b.set(5);
        b.set(6);
        ch[0] = static_cast<char>(b.to_ulong());
        plink.write(ch, 1);
        b.reset();
        b.set(0);
        b.set(1);
        b.set(3);
        b.set(4);
        ch[0] = static_cast<char>(b.to_ulong());
        plink.write(ch, 1);
        b.reset();
        b.set(0);
        ch[0] = static_cast<char>(b.to_ulong());
        plink.write(ch, 1);
        for (auto&& g : geno)
        {
            size_t c = 0;
            size_t i = 0;
            while (i < g.size())
            {
                c = 0;
                b.reset();
                while (c < 8 && i < g.size())
                {
                    switch (g[i])
                    {
                    case 2:
                        b.set(c);
                        b.set(c + 1);
                        break;
                    case 1: b.set(c + 1); break;
                    case 0: break;
                    case 3: b.set(c); break;
                    }
                    c += 2;
                    ++i;
                }
                ch[0] = static_cast<char>(b.to_ulong());
                plink.write(ch, 1);
            }
        }
        plink.close();
    }

    void gen_fake_bed(const std::vector<size_t>& geno, const std::string& name)
    {
        gen_fake_bed(std::vector<std::vector<size_t>> {geno}, name);
    }
    void set_sample_vector(const size_t n_sample)
    {
        for (size_t i = 0; i < n_sample; ++i)
        { SET_BIT(i, m_calculate_prs.data()); }
        m_sample_ct = n_sample;
    }

    void set_sample_vector(const std::vector<bool>& selected_samples)
    {
        m_unfiltered_sample_ct = selected_samples.size();
        const uintptr_t unfiltered_sample_ctl =
            BITCT_TO_WORDCT(m_unfiltered_sample_ct);
        m_calculate_prs.resize(unfiltered_sample_ctl, 0);
        m_sample_ct = 0;
        for (size_t i = 0; i < m_unfiltered_sample_ct; ++i)
        {
            if (selected_samples[i])
            {
                SET_BIT(i, m_calculate_prs.data());
                ++m_sample_ct;
            }
        }
    }
    void set_founder_vector(const std::vector<bool>& founder)
    {
        m_founder_ct = 0;
        for (size_t i = 0; i < founder.size(); ++i)
        {
            if (founder[i])
            {
                SET_BIT(i, m_sample_for_ld.data());
                ++m_founder_ct;
            }
        }
    }

    void gen_bed_head(const std::string& name, size_t num_sample,
                      size_t num_snp, bool new_version, bool sample_major)
    {
        std::ofstream out(name.c_str(), std::ios::out | std::ios::binary);
        uintptr_t unfiltered_sample_ct4 = (num_sample + 3) / 4;
        std::bitset<8> b;
        char ch[1];
        if (new_version)
        {
            b.reset();
            b.set(2);
            b.set(3);
            b.set(5);
            b.set(6);
            ch[0] = static_cast<char>(b.to_ulong());
            out.write(ch, 1);
            b.reset();
            b.set(0);
            b.set(1);
            b.set(3);
            b.set(4);
            ch[0] = static_cast<char>(b.to_ulong());
            out.write(ch, 1);
        }
        b.reset();
        if (!sample_major) { b.set(0); }
        ch[0] = static_cast<char>(b.to_ulong());
        out.write(ch, 1);
        for (size_t i = 0; i < unfiltered_sample_ct4 * num_snp; ++i)
        { out.write(ch, 1); }
        out.close();
    }

    void test_single_marker_freqs_and_hwe(const std::vector<size_t>& geno,
                                          uint32_t* ll_ctp, uint32_t* lh_ctp,
                                          uint32_t* hh_ctp, uint32_t* ll_ctfp,
                                          uint32_t* lh_ctfp, uint32_t* hh_ctfp)
    {
        size_t geno_idx = 0;
        for (size_t i = 0; i < geno.size(); ++i)
        {
            switch (geno[i])
            {
            case 0: break;
            case 1: SET_BIT(geno_idx + 1, m_tmp_genotype.data()); break;
            case 2:
                SET_BIT(geno_idx, m_tmp_genotype.data());
                SET_BIT(geno_idx + 1, m_tmp_genotype.data());
                break;
            case 3: SET_BIT(geno_idx, m_tmp_genotype.data()); break;
            }
            geno_idx += 2;
        }
        const uintptr_t unfiltered_sample_ctl =
            BITCT_TO_WORDCT(m_unfiltered_sample_ct);
        const uintptr_t unfiltered_sample_ctv2 = 2 * unfiltered_sample_ctl;
        return single_marker_freqs_and_hwe(
            unfiltered_sample_ctv2, m_tmp_genotype.data(),
            m_sample_include2.data(), m_founder_include2.data(), m_sample_ct,
            ll_ctp, lh_ctp, hh_ctp, m_founder_ct, ll_ctfp, lh_ctfp, hh_ctfp);
    }
};

#endif // MOCK_BINARYPLINK_HPP
